#ifdef __linux__
#include <unistd.h>
#include <sys/syscall.h>
#endif
#include <iostream>
#include <iomanip>
#include <ctime>
#include <thread>
#include <cstdlib>
#include "thread_args.hpp"
namespace pcplusplus{
    void producer(thread_args *const args, const size_t target, size_t *const num_produced){
#ifdef __linux__
	const pid_t thread_id = syscall(SYS_gettid);
#else
	const std::thread::id thread_id = std::this_thread::get_id();
#endif
	long num;
	size_t index;
	std::unique_lock<std::mutex> mtx = args->getMutex();
	std::cout << "Producer thread " << thread_id << " started." << std::endl;
	mtx.unlock();
	while(*num_produced < target){
	    using namespace std::chrono_literals;
	    std::this_thread::sleep_for(10us);
	    mtx.lock();
	    while(args->buffer->isFull()){
		args->canProduce.wait(mtx);
	    }
	    if(*num_produced == target){
		std::cout << "Producer thread " << thread_id << " finished" << std::endl;
		mtx.unlock();
		return;
	    }
	    num = lrand48();
	    if(args->producerLog.is_open()){
		index = args->buffer->enqueue(num);
		struct timespec ts;
		timespec_get(&ts, TIME_UTC);
		args->producerLog << ts.tv_sec << std::setfill('0') << std::setw(9) << ts.tv_nsec;
		args->producerLog << " Producer " << thread_id << " " << index << " " << num << std::endl;
		std::cout << "Producer thread " << thread_id << " produced " << num << " and stored it at index " << index << std::endl;
	    }
	    else{
		std::cout << "Producer thread " << thread_id << " produced " << num << " and stored it at index " << args->buffer->enqueue(num) << std::endl;
	    }

	    ++*num_produced;
	    mtx.unlock();
	    args->canConsume.notify_all();
	}
	mtx.lock();
	std::cout << "Producer thread " << thread_id << " finished" << std::endl;
	mtx.unlock();
    }
}
