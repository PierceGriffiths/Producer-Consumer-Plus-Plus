#ifndef THREAD_ARGS_HPP
#define THREAD_ARGS_HPP
#include <fstream>
#include <condition_variable>
#include <mutex>
#include "queue.hpp"
#include "macro_defs.hpp"
#include "exceptions.hpp"
namespace pcplusplus{
    using tid_t = decltype(GET_TID);
    using ulock_t = std::unique_lock<std::mutex>;
    class thread_args{
	protected:
	    std::mutex mutex;
	    std::ofstream producerLog, consumerLog;
	    std::condition_variable canProduce, canConsume;
	    pcplusplus::queue *buffer;
	    size_t num_produced, num_consumed;

	public:
	    thread_args(const size_t capacity);
	    ~thread_args();
	    void openLogFiles() noexcept;
	    void wait_for_consumers();
	    void wait_for_producers();
	    void produce_item(const tid_t thread_id);
	    void consume_item(const tid_t thread_id);

	    ulock_t inline getMutex(){
		return ulock_t(this->mutex);
	    }

	    void inline lock(){
		this->mutex.lock();
	    }

	    void inline unlock(){
		this->mutex.unlock();
	    }

	    bool inline isFull() const noexcept{
		return this->buffer->isFull();
	    }

	    bool inline isEmpty() const noexcept{
		return this->buffer->isEmpty();
	    }

	    bool inline producerLog_is_open() const{
		return this->producerLog.is_open();
	    }

	    bool inline consumerLog_is_open() const{
		return this->consumerLog.is_open();
	    }
	    
	    void inline notify_consumers(){
		this->canConsume.notify_all();
	    }
	    
	    void inline notify_producers(){
		this->canProduce.notify_all();
	    }

	    size_t inline getNumProduced() const noexcept{
		return this->num_produced;
	    }

	    size_t inline getNumConsumed() const noexcept{
		return this->num_consumed;
	    }
    };
}
#endif
