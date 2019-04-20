#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include "thread_args.hpp"
#include "exceptions.hpp"
namespace pcplusplus{
    thread_args::thread_args(const size_t capacity){
	try{
	    buffer = new queue<long>(capacity);
	}
	catch(const exceptions::failed_init& e){
	    throw e;
	}
	catch(const std::bad_alloc& e){
	    e.~bad_alloc();
	    throw exceptions::failed_init("Failed to initialize queue instance");
	}
	srand48(std::time(nullptr));
    }

    thread_args::~thread_args(){
	delete buffer;
	if(producerLog.is_open())
	    producerLog.close();
	if(consumerLog.is_open())
	    consumerLog.close();
    }

    void thread_args::openLogFiles() noexcept{
	producerLog.exceptions(std::ofstream::badbit);
	try{
	    producerLog.open(PRODUCER_LOG_FILENAME, std::fstream::out | std::fstream::trunc);
	}
	catch(const std::ofstream::failure& e){
	    std::cerr << PRODUCER_LOG_FILENAME " could not be opened for writing" << std::endl;
	}
	consumerLog.exceptions(std::ofstream::badbit);
	try{
	    consumerLog.open(CONSUMER_LOG_FILENAME, std::fstream::out | std::fstream::trunc);
	}
	catch(const std::ofstream::failure& e){
	    std::cerr << CONSUMER_LOG_FILENAME " could not be opened for writing" << std::endl;
	}
    }

    void thread_args::produce_item(const tid_t thread_id){
	const long num = lrand48();
	const size_t index = this->buffer->enqueue(num);
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	if(producerLog.is_open() && producerLog)
	    producerLog << ts.tv_sec << std::setfill('0') << std::setw(9) << ts.tv_nsec << std::setw(0) <<
		" Producer " << thread_id << " " << index << " " << num << std::endl;

	std::cout << "Producer thread " << thread_id << " produced " << num << " and stored it at index " << index << std::endl;
    }

    void thread_args::consume_item(const tid_t thread_id){
	long num;
	const std::size_t index = this->buffer->dequeue(&num);
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	if(consumerLog.is_open() && consumerLog)
	    consumerLog << ts.tv_sec << std::setfill('0') << std::setw(9) << ts.tv_nsec << std::setw(0) <<
		" Consumer " << thread_id << " " << index << " " << num << std::endl;

	std::cout << "Consumer thread " << thread_id << " consumed " << num << " from index " << index << std::endl;
    }
}
