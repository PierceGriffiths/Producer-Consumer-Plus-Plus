#include <iostream>
#include <thread>
#include <memory>
#include <cerrno>
#include <cinttypes>
#include "macro_defs.hpp"
#include "threaded_functions.hpp"
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
            goto invalidargv;
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
invalidargv:
    std::cerr << "argument " << i << " (\'" << argv[i] << "\') not valid. Please provide a positive integer no greater than " << SIZE_MAX << std::endl;
    std::cerr << "Usage: " << argv[0] << " <# producer threads> <# consumer threads> <buffer size> <# items to produce>" << std::endl;
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
    
    std::unique_ptr<std::thread[]> producers;
    try{
	producers = std::make_unique<std::thread[]>(numProducers);
    }
    catch(const std::bad_alloc& e){
	delete tArgs;
	std::cerr << "Could not allocate memory for producer threads." << std::endl;
	return EXIT_FAILURE;
    }
    std::unique_ptr<std::thread[]> consumers;
    try{
	consumers = std::make_unique<std::thread[]>(numConsumers);
    }
    catch(const std::bad_alloc& e){
	producers.reset();
	delete tArgs;
	std::cerr << "Could not allocate memory for consumer threads." << std::endl;
	return EXIT_FAILURE;
    }
    
    tArgs->openLogFiles();
    
    size_t num_produced = 0, num_consumed = 0;
    srand48(std::time(nullptr));
    std::unique_lock<std::mutex> mtx = tArgs->getMutex();
    if(numProducers >= numConsumers){
	size_t i = 0;
	for(; i < numConsumers; ++i){
	    try{
		producers[i] = std::thread(pcplusplus::producer, tArgs, target, &num_produced);
	    }
	    catch(const std::system_error& e){
		mtx.lock();
		std::cerr << "Failed to fork a producer thread." << std::endl;
		return EXIT_FAILURE;
	    }
	    
	    try{
		consumers[i] = std::thread(pcplusplus::consumer, tArgs, target, &num_consumed);
	    }
	    catch(const std::system_error& e){
		mtx.lock();
		std::cerr << "Failed to fork a consumer thread." << std::endl;
		return EXIT_FAILURE;
	    }
	}
	
	for(;i < numProducers; ++i){//if numProducers == numConsumers, this loop will be skipped
	    try{
		producers[i] = std::thread(pcplusplus::producer, tArgs, target, &num_produced);
	    }
	    catch(const std::system_error& e){
		mtx.lock();
		std::cerr << "Failed to fork a producer thread." << std::endl;
		return EXIT_FAILURE;
	    }
	}
	
	mtx.unlock();
	
	for(i = 0; i < numConsumers; ++i){
	    try{
		producers[i].join();
	    }
	    catch(const std::system_error& e){
		mtx.lock();
		std::cerr << "Error occured while attempting to join a producer thread." << std::endl;
		return EXIT_FAILURE;
	    }
	    
	    try{
		consumers[i].join();
	    }
	    catch(const std::system_error& e){
		mtx.lock();
		std::cerr << "Error occured while attempting to join a consumer thread." << std::endl;
		return EXIT_FAILURE;
	    }
	}
	consumers.reset();
	for(;i < numProducers; ++i){
	    try{
		producers[i].join();
	    }
	    catch(const std::system_error& e){
		std::cerr << "Error occured while attempting to join a producer thread." << std::endl;
		return EXIT_FAILURE;
	    }
	}
	
	producers.reset();
    }//end of if(numProducers >= numConsumers)
    else{
	size_t i = 0;
	for(; i < numProducers; ++i){
	    try{
		producers[i] = std::thread(pcplusplus::producer, tArgs, target, &num_produced);
	    }
	    catch(const std::system_error& e){
		mtx.lock();
		std::cerr << "Failed to fork a producer thread." << std::endl;
		return EXIT_FAILURE;
	    }

	    try{
		consumers[i] = std::thread(pcplusplus::consumer, tArgs, target, &num_consumed);
	    }
	    catch(const std::system_error& e){
		std::cerr << "Failed to fork a consumer thread." << std::endl;
		return EXIT_FAILURE;
	    }
	}
	
	for(;i < numConsumers; ++i){
	    try{
		consumers[i] = std::thread(pcplusplus::consumer, tArgs, target, &num_consumed);
	    }
	    catch(const std::system_error& e){
		mtx.lock();
		std::cerr << "Failed to fork a consumer thread." << std::endl;
		return EXIT_FAILURE;
	    }
	}
	
	mtx.unlock();
	
	for(i = 0; i < numProducers; ++i){
	    try{
		producers[i].join();
	    }
	    catch(const std::system_error& e){
		mtx.lock();
		std::cerr << "Error occured while attempting to join a producer thread." << std::endl;
		return EXIT_FAILURE;
	    }

	    try{
		consumers[i].join();
	    }
	    catch(const std::system_error& e){
		std::cerr << "Error occured while attempting to join a consumer thread." << std::endl;
		return EXIT_FAILURE;
	    }
	}
	
	producers.reset();
	
	for(;i < numConsumers; ++i){
	    try{
		consumers[i].join();
	    }
	    catch(const std::system_error& e){
		mtx.lock();
		std::cerr << "Error occured while attempting to join a consumer thread." << std::endl;
		return EXIT_FAILURE;
	    }
	}
	consumers.reset();
    }//end of else
    
    mtx.~unique_lock();
    bool pLogged = tArgs->producerLog.is_open(), cLogged = tArgs->consumerLog.is_open();
    delete tArgs;
    std::cout << "Number of items produced: " << num_produced << std::endl << "Number of items consumed: " << num_consumed << std::endl;
    
    if(pLogged){
	std::ifstream producerLog;
	producerLog.exceptions(std::ifstream::failbit);
	std::cout << "Beginning of " PRODUCER_LOG_FILENAME << std::endl;
	try{
	    std::string buff;
	    producerLog.open(PRODUCER_LOG_FILENAME);
	    while(std::getline(producerLog, buff))
		std::cout << buff << std::endl;
	}
	catch(const std::ifstream::failure& e){
	    std::cout << "End of " PRODUCER_LOG_FILENAME << std::endl;
	}
	if(producerLog.is_open())
	    producerLog.close();
    }
    if(cLogged){
	std::ifstream consumerLog;
	consumerLog.exceptions(std::ifstream::failbit);
	std::cout << "Beginning of " CONSUMER_LOG_FILENAME << std::endl;
	try{
	    std::string buff;
	    consumerLog.open(CONSUMER_LOG_FILENAME);
	    while(std::getline(consumerLog, buff))
		std::cout << buff << std::endl;
	}
	catch(const std::ifstream::failure& e){
	    std::cout << "End of " CONSUMER_LOG_FILENAME << std::endl;
	}
	if(consumerLog.is_open())
	    consumerLog.close();
    }
    
    return EXIT_SUCCESS;
}//end of main()
