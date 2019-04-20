#include <iostream>
#include <chrono>
#include "threaded_functions.hpp"
namespace pcplusplus{
    void producer_threads::producer_thread(thread_args *args){
	const auto thread_id = GET_TID;
	lock();
	std::cout << "Producer thread " << thread_id << " started." << std::endl;
	unlock();
	while(num_processed < target){
	    std::this_thread::sleep_for(std::chrono::microseconds(5));//sleep for 5 microseconds so that other producer threads have an opportunity to acquire the mutex
	    lock();
	    while(args->isFull()){
		wait();
	    }
	    if(num_processed == target){
		std::cout << "Producer thread " << thread_id << " finished." << std::endl;
		unlock();
		return;
	    }

	    args->produce_item(thread_id);
	    ++num_processed;
	    unlock();
	    notify();
	}
	lock();
	std::cout << "Producer thread " << thread_id << " finished." << std::endl;
	unlock();
    }
}
