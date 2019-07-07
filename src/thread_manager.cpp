#include <iostream>
#include "thread_wrappers.hpp"
#include "thread_manager.hpp"

namespace pcplusplus{
    void thread_manager::run_threads(std::size_t num_producers, std::size_t num_consumers, std::size_t buffer_capacity, std::size_t target){
	queue<long> *buffer;
	try{
            buffer = new queue<long>(buffer_capacity);
        }
        catch(const std::bad_alloc& e){
	    std::cerr << "Failed to initialize buffer" << std::endl;
	    std::exit(EXIT_FAILURE);
        }
	std::mutex mtx;
	std::condition_variable canProduce, canConsume;
	threads::generic_threads *producers, *consumers;
	try{
	    producers = new threads::producer_threads(target, num_producers, &mtx, &canProduce, &canConsume, buffer);
	}
	catch(const std::bad_alloc& e){
	    std::cerr << "Failed to allocate memory for producer threads." << std::endl;
	    std::exit(EXIT_FAILURE);
	}

	try{
	    consumers = new threads::consumer_threads(target, num_consumers, &mtx, &canConsume, &canProduce, buffer);
	}
	catch(const std::bad_alloc& e){
	    std::cerr << "Failed to allocate memory for consumer threads." << std::endl;
	    std::exit(EXIT_FAILURE);
	}
	
	size_t num_produced, num_consumed;
	try{
	    producers->fork();
	    consumers->fork();
	    num_produced = producers->join();
	    num_consumed = consumers->join();
	}
	catch(const exceptions::thread_failure& e){
	    std::cerr << e.what() << std::endl;
	    std::exit(EXIT_FAILURE);
	}
	
	last_producer_log_status = producers->logfile_is_open();
	delete producers;

	last_consumer_log_status = consumers->logfile_is_open();
	delete consumers;

	delete buffer;

	std::cout << "All threads finished." << std::endl;
	std::cout << "Produced: " << num_produced << std::endl << "Consumed: " << num_consumed << std::endl << std::endl;
    }
}
