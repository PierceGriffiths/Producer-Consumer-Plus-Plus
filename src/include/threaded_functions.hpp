#ifndef THREADED_FUNCTIONS_HPP
#define THREADED_FUNCTIONS_HPP
#include "thread_args.hpp"
namespace pcplusplus{

    void producer(thread_args *const args, const size_t target);

    void consumer(thread_args *const args, const size_t target);

} //end of namespace block

#endif //ifndef THREADED_FUNCTIONS_HPP
