#ifndef THREAD_MANAGER_HPP
#define THREAD_MANAGER_HPP
#include <cstddef>

namespace pcplusplus{
    class thread_manager{
	private:
	    bool last_producer_log_status = false, last_consumer_log_status = false;
	public:
	    void run_threads(std::size_t num_producers, std::size_t num_consumers, std::size_t buffer_capacity, std::size_t target);

	    inline bool producer_log_was_written() const noexcept{
		return last_producer_log_status;
	    }

	    inline bool consumer_log_was_written() const noexcept{
		return last_consumer_log_status;
	    }
    };
}
#endif
