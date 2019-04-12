#include <iostream>
#include "thread_args.hpp"
#include "exceptions.hpp"
#include "macro_defs.hpp"
namespace pcplusplus{
    thread_args::thread_args(const size_t capacity){
	try{
	    buffer = new queue(capacity);
	}
	catch(const exceptions::failed_init& e){
	    throw e;
	}
	catch(const std::bad_alloc& e){
	    e.~bad_alloc();
	    throw exceptions::failed_init("Failed to initialize queue instance");
	}
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
}
