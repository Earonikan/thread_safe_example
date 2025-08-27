#include "libs/utils/include/printer.hpp"
#include "include/core/task.hpp"
#include "include/core/worker.hpp"

#include "libs/ts_queue/ts_blocking_queue.hpp"
// #include "src/core/ts_mpmc_queue.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <optional>

// #include <boost/lockfree/queue.hpp>

namespace ts_queue {

void test_task() {
    auto task = std::make_shared<Task>("Greeting");
    auto printer = std::make_shared<Printer>();

    printer->SetMessage("Hello Gregory");

    task->SetCallback<Printer>(&Printer::Print, printer);

    task->Invoke();

}

void test_Worker() {

    // class Producer : public Worker {

    // public:
    //     Producer();
    // };

    TS_BlockingQueue<Task> queue;

    Task calculation_task("Generate Random Number");
    calculation_task.SetCallback<void>([&]() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> workload_dist(50, 200);
        int workload = workload_dist(gen);
        std::cout << workload << std::endl;
        // Simulate time between task generation
        std::this_thread::sleep_for(std::chrono::milliseconds(workload));
    });

    // calculation_task.Invoke();

    Task push_task("Pushing to queue");
    push_task.SetCallback<void>([&]() {
        queue.push(calculation_task);

    });

    Task pop_task("Poping from queue and process");
    pop_task.SetCallback<void>([&]() {
        Task task;
        while (!queue.empty()) {
            queue.pop(task);
            task.Invoke();
        }
    });

    Worker producer1;
    producer1.SetTask(push_task, 1);

    // uint64_t pool_size = 1;
    // std::vector<Worker> worker_pool(pool_size);

    // for (uint64_t i{0}; i < pool_size; ++i) {
    //     Worker consumer;
    //     consumer.SetTask(pop_task, 1);
    //     // worker_pool.push_back(consumer);
    // }
    
    // producer1.DoTask();

    // for (uint64_t i{0}; i < pool_size; ++i) {
    //     worker_pool[i].DoTask();
    // }

    // consumer.DoTask();

}

// void test() {
        
//     LockFreeMPMCQueue<int> queue(1024); // Power-of-2 capacity

//     // Producer thread
//     auto producer = [&] {
//         for (int i = 0; i < 100; ++i) {
//             while (!queue.try_push(i)) {
//                 std::cout << "pushing " << i << std::endl; 
//                 std::this_thread::yield(); // Or use blocking wrapper
//             }
//         }
//     };
//     // Consumer thread
//     auto consumer = [&] {
//         int value;
//         for (int i = 0; i < 100; ++i) {
//             while (!queue.try_pop(value)) {
//                 std::cout << "poping " << i << std::endl;
//                 std::this_thread::yield();
//             }
//             // Process value
//         }
//     };
//     // Run 4 producers and 4 consumers
//     std::vector<std::thread> prods, cons;
//     for (int i = 0; i < 1; ++i) {
//         prods.emplace_back(producer);
//         cons.emplace_back(consumer);
//     }
//     for (auto& t : prods) t.join();
//     for (auto& t : cons) t.join();
// }
}


// int main(int argv, char* argc[]) {
int main() {

    auto startTime = std::chrono::high_resolution_clock::now();

    ts_queue::test_Worker();

    // ts_queue::test_task();

    auto endTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();
    std::cout << "\nTotal elapsed time: " << elapsedTime << " seconds" << std::endl;
    
    // ts_queue::Printer printer;
    // printer.SetMessage("Hello Gregory");
    // printer.Print();

    // ts_queue::ts_queue_mutex<std::string> q;
    // // boost::lockfree::spsc_queue<std::string, boost::lockfree::capacity<1024>> q;

    // const int N = 10;

    // srand(static_cast<uint>(time(NULL)));

    // // sender 1
    // std::thread snd1([&](){
    //     // std::this_thread::sleep_for(std::chrono::microseconds(10 * rand() % 100));
    //     int num;
    //     std::string str;
    //     for (auto i = 0; i < N; ++i) {
    //         str = "sender 1, msg " + std::to_string(i);
    //         q.push(str);
    //         num = 10 * rand() % 100;
    //         std::this_thread::sleep_for(std::chrono::microseconds(num));
    //     }
    // });

    // // sender 2
    // std::thread snd2([&](){
    //     // std::this_thread::sleep_for(std::chrono::microseconds(10 * rand() % 100));
    //     int num;
    //     std::string str;
    //     for (auto i = 0; i < N; ++i) {
    //         str = "sender 2, msg " + std::to_string(i);
    //         q.push(str);
    //         num = 10 * rand() % 100;
    //         std::this_thread::sleep_for(std::chrono::microseconds(num));
    //     }
    // });

    // // receiver 1
    // std::thread rcv1([&](){
    //     std::this_thread::sleep_for(std::chrono::microseconds(1000));
    //     std::string str;
    //     while (!q.empty()) {
    //         // if (!q.empty()) {
    //             q.pop(str);
    //             std::cout << "rcv 1 pop: " << str << std::endl;
    //         // }
    //             // auto val = q.pop();
    //             // if (val.has_value()) {
    //                 // std::cout << "rcv 1 pop: " << val.value() << std::endl;
    //             // }
    //         std::this_thread::sleep_for(std::chrono::microseconds(1000));
    //     }
    // });

    // // receiver 2
    // std::thread rcv2([&](){
    //     // std::this_thread::sleep_for(std::chrono::microseconds(800));
    //     std::string str;
    //     while (!q.empty()) {
    //         // if (!q.empty()) {
    //             q.pop(str);
    //             std::cout << "rcv 2 pop: " << str << std::endl;
    //         // }
    //             // auto val = q.pop();
    //             // if (val.has_value()) {
    //                 // std::cout << "rcv 2 pop: " << val.value() << std::endl;
    //             // }
    //         std::this_thread::sleep_for(std::chrono::microseconds(1000));
    //     }
    // });
    
    // snd1.join();
    // snd2.join();

    // rcv1.join();
    // rcv2.join();

    return 0;
}