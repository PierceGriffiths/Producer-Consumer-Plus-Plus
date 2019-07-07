#ifndef PC_PLUS_PLUS_EXCEPTIONS_HPP
#define PC_PLUS_PLUS_EXCEPTIONS_HPP
#include <stdexcept>
namespace pcplusplus::exceptions{
    class failed_init : public std::runtime_error{//thrown within the thread_args and queue constructors when an std::bad_alloc is caught in order to better describe the failure point
	public:
	    explicit failed_init(const char* what_arg) : std::runtime_error(what_arg){
	    }

	    explicit failed_init(const std::string& what_arg) : std::runtime_error(what_arg){
	    }
    };//end of class failed_init

    class thread_failure : public std::runtime_error{//thrown when forking or joining a thread fails
	public:
	    explicit thread_failure(const char* what_arg) : std::runtime_error(what_arg){
	    }

	    explicit thread_failure(const std::string& what_arg) : std::runtime_error(what_arg){
	    }
    };

}//end of namespace block
#endif
