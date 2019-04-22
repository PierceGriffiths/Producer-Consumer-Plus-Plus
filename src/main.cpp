#include <iostream>
#include <thread>
#include <memory>
#include <cerrno>
#include <cinttypes>
#include "macro_defs.hpp"
#include "thread_wrappers.hpp"
#include "exceptions.hpp"

pcplusplus::thread_args* checkArguments(const char *argv[], size_t *target, size_t *numProducers, size_t *numConsumers){
    pcplusplus::thread_args *tArgs = nullptr;
    size_t argCheck;
    unsigned char i = 1;
    for(; i < 5; ++i){//Ensure all provided arguments are valid
	if(argv[i][0] == '-')
	    goto invalidargv;
	errno = 0;
	argCheck = std::strtoumax(argv[i], NULL, 10);
	if(errno == ERANGE || argCheck == 0){
invalidargv:
	    std::cerr << "argument " << i << " (\'" << argv[i] << "\') not valid. Please provide a positive integer no greater than " << SIZE_MAX << std::endl;
	    std::cerr << "Usage: " << argv[0] << " <# producer threads> <# consumer threads> <buffer size> <# items to produce>" << std::endl;
	    break;
        }
        switch(i){
            case 1:
                *numProducers = argCheck;
                continue;
            case 2:
                *numConsumers = argCheck;
                continue;
            case 3:
		try{
		    tArgs = new pcplusplus::thread_args(argCheck);
		}
		catch(const pcplusplus::exceptions::failed_init& e){
		    std::cerr << e.what() << std::endl;
		    return nullptr;
		}
		catch(const std::bad_alloc& e){
		    std::cerr << "Failed to initialize thread arguments" << std::endl;
		    return nullptr;
		}
                continue;
            case 4:
		*target = argCheck;
		return tArgs;
        }
    }
    return nullptr;
}

int main(const int argc, const char *argv[]){
    if(argc != 5){
	std::cerr << "Usage: " << argv[0] << " <# producer threads> <# consumer threads> <buffer size> <# items to produce>" << std::endl;
	return EXIT_FAILURE;
    }
    size_t target = 0, numProducers = 0, numConsumers = 0;
    pcplusplus::thread_args *tArgs = checkArguments(argv, &target, &numProducers, &numConsumers);
    if(tArgs == nullptr)
	return EXIT_FAILURE;
    
    std::mutex mutex;
    std::condition_variable canProduce, canConsume;
    pcplusplus::generic_threads *producers, *consumers;
    try{
	producers = new pcplusplus::producer_threads(target, numProducers, &mutex, &canProduce, &canConsume);
    }
    catch(const std::bad_alloc& e){
	std::cerr << "Could not allocate memory for producer threads." << std::endl;
	return EXIT_FAILURE;
    }

    try{
	consumers = new pcplusplus::consumer_threads(target, numProducers, &mutex, &canConsume, &canProduce);
    }
    catch(const std::bad_alloc& e){
	std::cerr << "Could not allocate memory for consumer threads." << std::endl;
	return EXIT_FAILURE;
    }

    tArgs->openLogFiles();
    
    producers->fork(tArgs);
    consumers->fork(tArgs);

    const size_t num_produced = producers->join();
    delete producers;
    
    const size_t num_consumed = consumers->join();
    delete consumers;

    const bool pLogged = tArgs->producerLog_is_open(), cLogged = tArgs->consumerLog_is_open();
    delete tArgs;
    std::cout << "All threads finished." << std::endl;
    std::cout << "Produced: " << num_produced << std::endl << "Consumed: " << num_consumed << std::endl << std::endl;
    
    if(pLogged){
	std::ifstream producerLog;
	producerLog.exceptions(std::ifstream::failbit);
	std::string buff;
	try{
	    producerLog.open(PRODUCER_LOG_FILENAME);
	    std::cout << "Reading from " PRODUCER_LOG_FILENAME ":" << std::endl;
	    while(std::getline(producerLog, buff))
		std::cout << buff << std::endl;
	}
	catch(const std::ifstream::failure& e){
	    if(producerLog.eof())
		std::cout << "End of " PRODUCER_LOG_FILENAME << std::endl << std::endl;
	    else
		std::cerr << "An error occurred while attempting to read from " PRODUCER_LOG_FILENAME << std::endl << std::endl;
	}
	if(producerLog.is_open())
	    producerLog.close();
    }
    else{
	std::cerr << PRODUCER_LOG_FILENAME " was not written to, so it will not be read." << std::endl << std::endl;
    }

    if(cLogged){
	std::ifstream consumerLog;
	consumerLog.exceptions(std::ifstream::failbit);
	std::string buff;
	try{
	    consumerLog.open(CONSUMER_LOG_FILENAME);
	    std::cout << "Reading from " CONSUMER_LOG_FILENAME ":" << std::endl;
	    while(std::getline(consumerLog, buff))
		std::cout << buff << std::endl;
	}
	catch(const std::ifstream::failure& e){
	    if(consumerLog.eof())
		std::cout << "End of " CONSUMER_LOG_FILENAME << std::endl;
	    else
		std::cerr << "An error occurred while attempting to read from " CONSUMER_LOG_FILENAME << std::endl;
	}
	if(consumerLog.is_open())
	    consumerLog.close();
    }
    else{
	std::cerr << CONSUMER_LOG_FILENAME " was not written to, so it will not be read." << std::endl;
    }
    
    return EXIT_SUCCESS;
}//end of main()
