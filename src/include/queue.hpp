#ifndef QUEUE_HPP
#define QUEUE_HPP
namespace pcplusplus{
    template<typename T>
    class queue{
	protected:
	    size_t front_index, back_index, current_size;
	    const size_t capacity;
	    std::unique_ptr<T[]> array;
	public:
	    explicit queue(const size_t cap) : capacity(cap){
		array = std::make_unique<T[]>(this->capacity);
		back_index = this->capacity - 1;
		front_index = 0;
		current_size = 0;
	    }

	    ~queue(){
		array.reset(nullptr);
	    }

	    inline size_t enqueue(const T num) noexcept{
		back_index = (back_index + 1) % capacity;
		array[back_index] = num;
		++current_size;
		return back_index;
	    }

	    inline size_t dequeue(T *const num) noexcept{
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
