#include <iostream>
#include <thread>
#include <chrono>
#include "thread_args.hpp"
namespace pcplusplus{
    void producer(thread_args *const args, const size_t target){
	const auto thread_id = GET_TID;
	args->lock();
	std::cout << "Producer thread " << thread_id << " started." << std::endl;
	args->unlock();
	while(args->getNumProduced() < target){
	    std::this_thread::sleep_for(std::chrono::microseconds(5));//sleep for 5 microseconds so that other producer threads have an opportunity to acquire the mutex
	    args->lock();
	    while(args->isFull()){
		args->wait_for_consumers();
	    }
	    if(args->getNumProduced() == target){
		std::cout << "Producer thread " << thread_id << " finished." << std::endl;
		args->unlock();
		return;
	    }

	    args->produce_item(thread_id);
	    args->unlock();
	    args->notify_consumers();
	}
	args->lock();
	std::cout << "Producer thread " << thread_id << " finished." << std::endl;
	args->unlock();
    }
}
