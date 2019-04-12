#include "queue.hpp"
#include "exceptions.hpp"
namespace pcplusplus{
    queue::queue(const size_t capacity) : capacity(capacity){
	if(this->capacity == 0)
	    throw std::invalid_argument("Cannot create a queue with zero capacity");
	try{
	    array = std::make_unique<long[]>(this->capacity);
	}
	catch(const std::bad_alloc& e){
	    throw exceptions::failed_init("Failed to allocate space for queue data structure");
	}
	back_index = this->capacity - 1;
	front_index = 0;
	current_size = 0;
    }

    queue::~queue(){
	array.reset();
    }

    size_t queue::enqueue(const long num){
	if(this->isFull())
	    throw exceptions::enqueue_full();
	back_index = (back_index + 1) % capacity;
	array[back_index] = num;
	++current_size;
	return back_index;
    }

    size_t queue::dequeue(long *const num){
	if(this->isEmpty())
	    throw exceptions::dequeue_empty();
	size_t old_index = front_index;
	*num = array[front_index];
	front_index = (front_index + 1) % capacity;
	--current_size;
	return old_index;
    }
}
