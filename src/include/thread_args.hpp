#ifndef THREAD_ARGS_HPP
#define THREAD_ARGS_HPP
#include <fstream>
#include <mutex>
#include <condition_variable>
#include "queue.hpp"
namespace pcplusplus{
    class thread_args{
	protected:
	    std::mutex mutex;
	public:
	    std::ofstream producerLog, consumerLog;
	    std::condition_variable canProduce, canConsume;
	    pcplusplus::queue *buffer;

	    thread_args(const size_t capacity);

	    ~thread_args();

	    inline std::unique_lock<std::mutex> getMutex(){
		return std::unique_lock<std::mutex>(this->mutex);
	    }

	    void openLogFiles() noexcept;
    };
}
#endif
