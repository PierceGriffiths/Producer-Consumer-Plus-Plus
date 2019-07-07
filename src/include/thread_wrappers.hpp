#ifndef THREAD_WRAPPERS_HPP
#define THREAD_WRAPPERS_HPP
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "queue.hpp"
#include "exceptions.hpp"
#include "macro_defs.hpp"

namespace pcplusplus::threads{
    using ulock_t = std::unique_lock<std::mutex>;
    using C_V = std::condition_variable;
    class generic_threads{
	private:
	    std::mutex *mutex;
	    C_V *const waitCV, *const signalCV;
	    const std::string thread_type = "generic";
	protected:
	    const size_t num_threads, target;
	    size_t num_processed;
	    std::unique_ptr<std::thread[]> threads;
	    std::ofstream logfile;
	    queue<long> *buffer;

	    inline void lock(){
		mutex->lock();
	    }

	    inline void unlock(){
		mutex->unlock();
	    }

	    inline void wait(){
		ulock_t mtx(*mutex, std::adopt_lock);
		waitCV->wait(mtx);
		mtx.release();
	    }

	    inline void notify(){
		signalCV->notify_all();
	    }

	    virtual void thread_routine(){};
	public:
	    generic_threads(size_t target, size_t count, std::mutex *mtx, C_V *wait, C_V *sig, queue<long> *buff)
		: waitCV(wait), signalCV(sig), num_threads(count), target(target)
	    {
		logfile.exceptions(std::ofstream::badbit);
		threads = std::make_unique<std::thread[]>(num_threads);
		mutex = mtx;
		buffer = buff;
		num_processed = 0;
	    }

	    virtual ~generic_threads(){
		if(logfile.is_open())
		    logfile.close();
		threads.reset();
	    }

	    virtual inline void fork(){
		lock();
		try{
		    for(size_t i = 0; i < num_threads; ++i)
			threads[i] = std::thread(&generic_threads::thread_routine, this);
		}
		catch(const std::system_error& e){
		    throw exceptions::thread_failure("Failed to fork a " + thread_type + " thread.");
		}
		unlock();
	    }

	    virtual inline size_t join(){
		try{
		    for(size_t i = 0; i < num_threads; ++i)
			threads[i].join();
		}
		catch(const std::system_error& e){
		    throw exceptions::thread_failure("Failed to fork a " + thread_type + " thread.");
		}
		return num_processed;
	    }

	    inline bool logfile_is_open() const{
		return logfile.is_open();
	    }

	    inline void close_logfile(){
		if(logfile.is_open())
		    logfile.close();
	    }
    };//end of class generic_threads

    class producer_threads : public pcplusplus::threads::generic_threads{
	private:
	    const std::string thread_type = "producer";
	protected:
	    void thread_routine();
	public:
	    producer_threads(size_t target, size_t count, std::mutex *mtx, C_V *canPro, C_V *canCon, queue<long> *buff);

	    virtual ~producer_threads(){};

    };//end of class producer_threads

    class consumer_threads : public pcplusplus::threads::generic_threads{
	private:
	    const std::string thread_type = "consumer";
	protected:
	    void thread_routine();
	public:
	    consumer_threads(size_t target, size_t count, std::mutex *mtx, C_V *canCon, C_V *canPro, queue<long> *buff);

	    virtual ~consumer_threads(){};

    };//end of class consumer_threads


} //end of namespace block

#endif //ifndef THREAD_WRAPPERS_HPP
