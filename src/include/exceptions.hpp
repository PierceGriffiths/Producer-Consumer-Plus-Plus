#ifndef PC_PLUS_PLUS_EXCEPTIONS_HPP
#define PC_PLUS_PLUS_EXCEPTIONS_HPP
#include <stdexcept>
namespace pcplusplus{
    namespace exceptions{
	class failed_init : public std::bad_alloc{
	    protected:
		const char* msg;
	    public:
		failed_init(const char* msg) : std::bad_alloc(), msg(msg){
		}

		const char* what() const throw(){
		    return msg;
		}
	};//end of failed_init

	class enqueue_full : public std::logic_error{
	    public:
		enqueue_full() : std::logic_error("Called \"size_t Queue::enqueue(const long num)\" on a full Queue"){
		}
	};//end of enqueue_full

	class dequeue_empty : public std::logic_error{
	    public:
		dequeue_empty() : std::logic_error("Called \"size_t Queue::dequeue(long const* num)\" on an empty Queue"){
		}
	};//end of dequeue_empty
    }//end of namespace exceptions block
}//end of namespace pcplusplus block
#endif
