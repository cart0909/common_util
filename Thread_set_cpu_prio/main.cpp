#include <iostream>
#include <climits>
#include <vector>
#include <memory>
#include "tracer.h"
#include "ThreadPool.h"
using namespace std;

void foo() {
    ScopedTrace st("foo");
    vector<double> container;
    for(int i = 0; i < 100000; ++i) {
        container.emplace_back(i);
    }
}

int main() {

    pthread_t this_thread = pthread_self();

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    int rc = pthread_setaffinity_np(this_thread, sizeof(cpu_set_t), &cpuset);
    if (rc != 0) {
      std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
    }

    // set priority to the maximum.
    struct sched_param params;
    params.sched_priority = sched_get_priority_max(SCHED_FIFO);

    int ret;
    ret = pthread_setschedparam(this_thread, SCHED_FIFO, &params);
    if (ret != 0) {
       // Print the error
       std::cout << "Unsuccessful in setting thread realtime prio" << std::endl;
       return -1;
   }

    std::shared_ptr<ThreadPool> thread_pool = std::make_shared<ThreadPool>(3);
    thread_pool->setCore(0, 1);
    thread_pool->setCore(1, 2);
    thread_pool->setCore(2, 3);
    while(1) {
        ScopedTrace st("signal");
        for(int i = 0; i < 3; ++i)
            thread_pool->enqueue([]{foo();});
        std::this_thread::sleep_for(std::chrono::microseconds(16666));
    }

    return 0;
}
