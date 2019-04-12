#ifndef THREADED_FUNCTIONS_HPP
#define THREADED_FUNCTIONS_HPP
#include "thread_args.hpp"
namespace pcplusplus{

    void producer(thread_args *const args, const size_t target, size_t *const num_produced);

    void consumer(thread_args *const args, const size_t target, size_t *const num_consumed);
}

#endif
