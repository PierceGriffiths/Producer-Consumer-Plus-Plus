#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <memory>
#include "exceptions.hpp"
namespace pcplusplus{
    template<typename T>
    class queue{
	protected:
	    size_t front_index, back_index, current_size;
	    const size_t capacity;
	    std::unique_ptr<T[]> array;
	public:
	    queue(const size_t cap) : capacity(cap){
		if(this->capacity == 0)
		    throw std::invalid_argument("Cannot create a queue with zero capacity");
		try{
		    array = std::make_unique<T[]>(this->capacity);
		}
		catch(const std::bad_alloc& e){
		    throw exceptions::failed_init("Failed to allocate space for queue data structure");
		}
		back_index = this->capacity - 1;
		front_index = 0;
		current_size = 0;
	    }

	    ~queue(){
		array.reset();
	    }

	    size_t enqueue(const T num){
		if(this->isFull())
		    throw exceptions::enqueue_full();
		back_index = (back_index + 1) % capacity;
		array[back_index] = num;
		++current_size;
		return back_index;
	    }

	    size_t dequeue(T *const num){
		if(this->isEmpty())
		    throw exceptions::dequeue_empty();
		T old_index = front_index;
		*num = array[front_index];
		front_index = (front_index + 1) % capacity;
		--current_size;   
		return old_index;
	    }
	    
	    inline bool isEmpty() const noexcept{
		return current_size == 0;
	    }
	    
	    inline bool isFull() const noexcept{
		return current_size == capacity;
	    }
    };
}
#endif
