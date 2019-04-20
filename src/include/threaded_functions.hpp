#ifndef THREADED_FUNCTIONS_HPP
#define THREADED_FUNCTIONS_HPP
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "thread_args.hpp"
namespace pcplusplus{
    class generic_threads{
	protected:
	    using ulock_t = std::unique_lock<std::mutex>;
	    using C_V = std::condition_variable;
	private:
	    std::mutex *mutex;
	    C_V *const waitCV, *const signalCV;
	protected:
	    const size_t num_threads, target;
	    size_t num_processed;
	    std::unique_ptr<std::thread[]> threads;

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

	    virtual void thread_routine(thread_args *tArgs){};
	public:
	    generic_threads(size_t target, size_t count, std::mutex *mtx, C_V *wait, C_V *sig) : waitCV(wait), signalCV(sig), num_threads(count), target(target){
		threads = std::make_unique<std::thread[]>(num_threads);
		mutex = mtx;
		num_processed = 0;
	    }

	    virtual ~generic_threads(){
		threads.reset();
	    }

	    virtual void fork(thread_args *tArgs){
		lock();
		for(size_t i = 0; i < num_threads; ++i)
		    threads[i] = std::thread(&generic_threads::thread_routine, this, tArgs);
		unlock();
	    }

	    virtual size_t join(){
		for(size_t i = 0; i < num_threads; ++i)
		    threads[i].join();
		return num_processed;
	    }
    };

    class producer_threads : public pcplusplus::generic_threads{
	private:
	    void producer_thread(thread_args *args);
	protected:
	    virtual void thread_routine(thread_args *tArgs){
		producer_thread(tArgs);
	    }
	public:
	    producer_threads(size_t target, size_t count, std::mutex *mtx, C_V *canPro, C_V *canCon) : generic_threads(target, count, mtx, canPro, canCon){
	    }
	    
	    virtual ~producer_threads(){};

	    void fork(thread_args *tArgs){
		try{
		    generic_threads::fork(tArgs);
		}
		catch(const std::system_error& e){
		    std::cerr << "Failed to fork a producer thread." << std::endl;
		    std::exit(EXIT_FAILURE);
		}
	    }

	    size_t join(){
		try{
		    return generic_threads::join();
		}
		catch(const std::system_error& e){
		    lock();
		    std::cerr << "Failed to join a producer thread." << std::endl;
		    std::exit(EXIT_FAILURE);
		}
	    }
    };

    class consumer_threads : public pcplusplus::generic_threads{
	private:
	    void consumer_thread(thread_args *args);
	protected:
	    virtual void thread_routine(thread_args *tArgs){
		consumer_thread(tArgs);
	    }
	public:
	    consumer_threads(size_t target, size_t count, std::mutex *mtx, C_V *canCon, C_V *canPro) : generic_threads(target, count, mtx, canCon, canPro){
	    }

	    virtual ~consumer_threads(){};

	    void fork(thread_args *tArgs){
		try{
		    generic_threads::fork(tArgs);
		}
		catch(const std::system_error& e){
		    std::cerr << "Failed to fork a consumer thread." << std::endl;
		    std::exit(EXIT_FAILURE);
		}
	    }

	    size_t join(){
		try{
		    return generic_threads::join();
		}
		catch(const std::system_error& e){
		    lock();
		    std::cerr << "Failed to join a consumer thread." << std::endl;
		    std::exit(EXIT_FAILURE);
		}
	    }
    };

} //end of namespace block

#endif //ifndef THREADED_FUNCTIONS_HPP
