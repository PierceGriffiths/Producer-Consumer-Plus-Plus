#ifndef THREAD_ARGS_HPP
#define THREAD_ARGS_HPP
#include <fstream>
#include "queue.hpp"
#include "macro_defs.hpp"
#include "exceptions.hpp"
namespace pcplusplus{
    using tid_t = decltype(GET_TID);
    class thread_args{
	protected:
	    std::ofstream producerLog, consumerLog;
	    pcplusplus::queue *buffer;

	public:
	    thread_args(const size_t capacity);
	    ~thread_args();
	    void openLogFiles() noexcept;
	    void produce_item(const tid_t thread_id);
	    void consume_item(const tid_t thread_id);

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
    };
}
#endif
