#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <memory>
namespace pcplusplus{
    class queue{
	protected:
	    size_t front_index, back_index, current_size;
	    const size_t capacity;
	    std::unique_ptr<long[]> array;
	public:
	    queue(const size_t cap);

	    ~queue();

	    size_t enqueue(const long num);

	    size_t dequeue(long *const num);
	    
	    inline bool isEmpty() noexcept{
		return current_size == 0;
	    }
	    
	    inline bool isFull() noexcept{
		return current_size == capacity;
	    }
    };
}
#endif
