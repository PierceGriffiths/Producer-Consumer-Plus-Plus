#include <iostream>
#include <iomanip>
#include <chrono>
#include "thread_wrappers.hpp"
namespace pcplusplus::threads{
    producer_threads::producer_threads(size_t target, size_t count, std::mutex *mtx, C_V *canPro, C_V *canCon, queue<long> *buff)
	: generic_threads(target, count, mtx, canPro, canCon, buff)
    {
	try{
	    logfile.open(PRODUCER_LOG_FILENAME, std::fstream::out | std::fstream::trunc);
	}
	catch(const std::ofstream::failure& e){
	    std::cerr << PRODUCER_LOG_FILENAME " could not be opened for writing" << std::endl;
	}
	srand48(std::time(nullptr));
    }

    void producer_threads::thread_routine(){
	const auto thread_id = GET_TID;
	struct timespec ts;
	lock();
	std::cout << "Producer thread " << thread_id << " started." << std::endl;
	unlock();
	while(num_processed < target){
	    std::this_thread::sleep_for(std::chrono::microseconds(5));//sleep for 5 microseconds so that other producer threads have an opportunity to acquire the mutex
	    lock();
	    while(buffer->isFull()){
		wait();
	    }
	    if(num_processed == target){
		std::cout << "Producer thread " << thread_id << " finished." << std::endl;
		unlock();
		return;
	    }

	    const long num = lrand48();
	    const size_t index = buffer->enqueue(num);
	    timespec_get(&ts, TIME_UTC);
	    if(logfile.is_open() && logfile)
		logfile << ts.tv_sec << std::setfill('0') << std::setw(9) << ts.tv_nsec << std::setw(0) << " Producer " << thread_id << " " << index << " " << num << std::endl;

	    std::cout << "Producer thread " << thread_id << " produced " << num << " and stored it at index " << index << std::endl;
	    ++num_processed;
	    unlock();
	    notify();
	}
	lock();
	std::cout << "Producer thread " << thread_id << " finished." << std::endl;
	unlock();
    }
}
