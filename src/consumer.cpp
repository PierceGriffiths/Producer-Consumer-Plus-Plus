#include <iostream>
#include <iomanip>
#include <chrono>
#include "thread_wrappers.hpp"
namespace pcplusplus::threads{
    consumer_threads::consumer_threads(size_t target, size_t count, std::mutex *mtx, C_V *canCon, C_V *canPro, queue<long> *buff)
	: generic_threads(target, count, mtx, canCon, canPro, buff)
    {
	try{
	    logfile.open(CONSUMER_LOG_FILENAME, std::fstream::out | std::fstream::trunc);
	}
	catch(const std::ofstream::failure& e){
	    std::cerr << CONSUMER_LOG_FILENAME " could not be opened for writing" << std::endl;
	}
    }


    void consumer_threads::thread_routine(){
	const auto thread_id = GET_TID;
	struct timespec ts;
	lock();
	std::cout << "Consumer thread " << thread_id << " started." << std::endl;
	unlock();
	while(num_processed < target){
	    std::this_thread::sleep_for(std::chrono::microseconds(5));//sleep for 5 microseconds so that other consumer threads have a chance to acquire the mutex
	    lock();
	    while(num_processed < target && buffer->isEmpty()){
		wait();
	    }
	    if(num_processed == target){
		std::cout << "Consumer thread " << thread_id << " finished." << std::endl;
		unlock();
		return;
	    }

	    long num;
	    const size_t index = buffer->dequeue(&num);
	    timespec_get(&ts, TIME_UTC);
	    if(logfile.is_open() && logfile)
		logfile << ts.tv_sec << std::setfill('0') << std::setw(9) << ts.tv_nsec << std::setw(0) << " Consumer " << thread_id << " " << index << " " << num << std::endl;

	    std::cout << "Consumer thread " << thread_id << " consumed " << num << " from index " << index << std::endl;
	    ++num_processed;
	    unlock();
	    notify();
	}
	lock();
	std::cout << "Consumer thread " << thread_id << " finished." << std::endl;
	unlock();
    }
}
