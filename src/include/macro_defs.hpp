#ifndef MACRO_DEFS_HPP
#define MACRO_DEFS_HPP

#define PRODUCER_LOG_FILENAME "producer-event.log"
#define CONSUMER_LOG_FILENAME "consumer-event.log"

#ifndef SIZE_MAX
#ifdef __SIZE_MAX__
#define SIZE_MAX __SIZE_MAX__
#else
#define SIZE_MAX (size_t)-1
#endif//ifdef __SIZE_MAX__
#endif//ifndef SIZE_MAX

#ifdef __linux__
#include <unistd.h>
#include <sys/syscall.h>
#define GET_TID syscall(SYS_gettid)
#else
#include <thread>
#define GET_TID std::this_thread::get_id()
#endif

#endif//ifndef MACRO_DEFS_HPP
