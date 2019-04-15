#ifndef PC_PLUS_PLUS_EXCEPTIONS_HPP
#define PC_PLUS_PLUS_EXCEPTIONS_HPP
#include <stdexcept>
namespace pcplusplus{
    namespace exceptions{
	class failed_init : public std::bad_alloc{//thrown within the thread_args and queue constructors when an std::bad_alloc is caught in order to better describe the failure point
	    protected:
		const char* msg;
	    public:
		failed_init(const char* msg) : std::bad_alloc(), msg(msg){
		}

		const char* what() const noexcept{
		    return msg;
		}
	};//end of class failed_init

	class enqueue_full : public std::logic_error{
	    public:
		enqueue_full() : std::logic_error("Called \"size_t Queue::enqueue(const long num)\" on a full Queue"){
		}
	};//end of class enqueue_full

	class dequeue_empty : public std::logic_error{
	    public:
		dequeue_empty() : std::logic_error("Called \"size_t Queue::dequeue(long const* num)\" on an empty Queue"){
		}
	};//end of class dequeue_empty
    }//end of namespace exceptions block
}//end of namespace pcplusplus block
#endif
