#ifdef __linux__
#include <unistd.h>
#include <sys/syscall.h>
#endif
#include <iostream>
#include <iomanip>
#include <ctime>
#include <thread>
#include "thread_args.hpp"
namespace pcplusplus{
    void consumer(thread_args *const args, const size_t target, size_t *const num_consumed){
#ifdef __linux__
	const pid_t thread_id = syscall(SYS_gettid);
#else
	const std::thread::id thread_id = std::this_thread::get_id();
#endif
	long num;
	size_t index;
	std::unique_lock<std::mutex> mtx = args->getMutex();
	std::cout << "Consumer thread " << thread_id << " started." << std::endl;
	mtx.unlock();
	while(*num_consumed < target){
	    using namespace std::chrono_literals;
	    std::this_thread::sleep_for(10us);
	    mtx.lock();
	    while(args->buffer->isEmpty() && *num_consumed < target){
		args->canConsume.wait(mtx);
	    }
	    if(*num_consumed == target){
		std::cout << "Consumer thread " << thread_id << " finished" << std::endl; 
		mtx.unlock();
		return;
	    }

	    index = args->buffer->dequeue(&num);
	    struct timespec ts;
	    timespec_get(&ts, TIME_UTC);
	    if(args->consumerLog.is_open()){
		args->consumerLog << ts.tv_sec << std::setfill('0') << std::setw(9) << ts.tv_nsec;
		args->consumerLog << " Consumer " << thread_id << " " << index << " " << num << std::endl;
	    }

	    std::cout << "Consumer thread " << thread_id << " consumed " << num << " from index " << index << std::endl;

	    ++*num_consumed;
	    mtx.unlock();
	    args->canProduce.notify_all();
	}
	mtx.lock();
	std::cout << "Consumer thread " << thread_id << " finished" << std::endl;
	mtx.unlock();
    }
}
