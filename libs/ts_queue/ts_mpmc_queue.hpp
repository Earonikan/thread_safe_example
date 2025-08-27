
#include <atomic>
#include <thread>
#include <vector>
#include <cstdint>
#include <type_traits>
#include <cassert>

namespace ts_queue {

    template <typename T>
    class LockFreeMPMCQueue {
    private:
        static constexpr size_t CACHE_LINE_SIZE = 64;
        static constexpr size_t MIN_CAPACITY = 2; // Must be >=2 for safety

        alignas(CACHE_LINE_SIZE) std::atomic<size_t> producer_head_;
        alignas(CACHE_LINE_SIZE) std::atomic<size_t> producer_tail_;
        alignas(CACHE_LINE_SIZE) std::atomic<size_t> consumer_head_;
        alignas(CACHE_LINE_SIZE) std::atomic<size_t> consumer_tail_;
        
        const size_t capacity_;
        T* buffer_;

        // Fast modulo for power-of-2 sizes
        size_t idx(size_t pos) const noexcept {
            return pos & (capacity_ - 1);
        }

        // Backoff strategy to reduce contention
        void backoff(size_t& spin_count) noexcept {
            if (++spin_count < 16) {
                std::this_thread::yield();
            } else {
                std::this_thread::sleep_for(std::chrono::nanoseconds(1));
            }
        }

    public:
        explicit LockFreeMPMCQueue(size_t capacity)
            : producer_head_(0), producer_tail_(0),
            consumer_head_(0), consumer_tail_(0),
            capacity_(capacity < MIN_CAPACITY ? MIN_CAPACITY : capacity),
            buffer_(new T[capacity_]) 
        {
            static_assert(std::is_nothrow_copy_constructible<T>::value ||
                        std::is_nothrow_move_constructible<T>::value,
                "T must be nothrow copy/move constructible");
            assert((capacity_ & (capacity_ - 1)) == 0 && "Capacity must be power of 2");
        }

        ~LockFreeMPMCQueue() {
            // Cleanup remaining items (if any)
            T value;
            while (try_pop(value)) {}
            delete[] buffer_;
        }

        bool try_push(const T& value) noexcept {
            return try_emplace(value);
        }

        bool try_push(T&& value) noexcept {
            return try_emplace(std::move(value));
        }

        template <typename... Args>
        bool try_emplace(Args&&... args) noexcept {
            size_t tail = producer_tail_.load(std::memory_order_relaxed);
            size_t next_tail = tail + 1;
            size_t head = producer_head_.load(std::memory_order_acquire);

            // Queue full? (account for wrap-around)
            if (next_tail - head >= capacity_) {
                size_t spin = 0;
                do {
                    head = producer_head_.load(std::memory_order_acquire);
                    if (next_tail - head < capacity_) break;
                    backoff(spin);
                } while (true);
            }

            // Reserve slot
            if (!producer_tail_.compare_exchange_weak(
                    tail, next_tail, 
                    std::memory_order_relaxed, 
                    std::memory_order_relaxed)) {
                return false; // Lost reservation race
            }

            // Construct in-place
            new (&buffer_[idx(tail)]) T(std::forward<Args>(args)...);
            
            // Publish to consumers
            while (!producer_head_.compare_exchange_weak(
                    head, next_tail, 
                    std::memory_order_release, 
                    std::memory_order_relaxed)) {
                head = producer_head_.load(std::memory_order_relaxed);
            }
            return true;
        }

        bool try_pop(T& value) noexcept {
            size_t head = consumer_head_.load(std::memory_order_relaxed);
            size_t next_head = head + 1;
            size_t tail = consumer_tail_.load(std::memory_order_acquire);

            // Queue empty? (account for wrap-around)
            if (head >= tail) {
                size_t spin = 0;
                do {
                    tail = consumer_tail_.load(std::memory_order_acquire);
                    if (head < tail) break;
                    backoff(spin);
                } while (true);
            }

            // Reserve item
            if (!consumer_head_.compare_exchange_weak(
                    head, next_head, 
                    std::memory_order_relaxed, 
                    std::memory_order_relaxed)) {
                return false; // Lost reservation race
            }

            // Copy out value
            value = std::move(buffer_[idx(head)]);
            buffer_[idx(head)].~T();

            // Publish to producers
            while (!consumer_tail_.compare_exchange_weak(
                    tail, next_head, 
                    std::memory_order_release, 
                    std::memory_order_relaxed)) {
                tail = consumer_tail_.load(std::memory_order_relaxed);
            }
            return true;
        }

        // Non-blocking size estimation (may be inaccurate under contention)
        size_t size() const noexcept {
            size_t head = producer_head_.load(std::memory_order_acquire);
            size_t tail = consumer_tail_.load(std::memory_order_acquire);
            return (head > tail) ? (head - tail) : 0;
        }

        bool empty() const noexcept {
            return size() == 0;
        }

        bool full() const noexcept {
            size_t head = producer_head_.load(std::memory_order_acquire);
            size_t tail = consumer_tail_.load(std::memory_order_acquire);
            return (tail + capacity_) <= head;
        }
    };
}