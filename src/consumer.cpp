#include <iostream>
#include <thread>
#include <chrono>
#include "thread_args.hpp"
namespace pcplusplus{
    void consumer(thread_args *const args, const size_t target){
	const auto thread_id = GET_TID;
	args->lock();
	std::cout << "Consumer thread " << thread_id << " started." << std::endl;
	args->unlock();
	while(args->getNumConsumed() < target){
	    std::this_thread::sleep_for(std::chrono::microseconds(5));//sleep for 5 microseconds so that other consumer threads have a chance to acquire the mutex
	    args->lock();
	    while(args->getNumConsumed() < target && args->isEmpty()){
		args->wait_for_producers();
	    }
	    if(args->getNumConsumed() == target){
		std::cout << "Consumer thread " << thread_id << " finished." << std::endl;
		args->unlock();
		return;
	    }

	    args->consume_item(thread_id);
	    args->unlock();
	    args->notify_producers();
	}
	args->lock();
	std::cout << "Consumer thread " << thread_id << " finished." << std::endl;
	args->unlock();
    }
}
