cmake_minimum_required(VERSION 3.13.4 FATAL_ERROR)
project("Producer Consumer++")
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR} CACHE PATH "" FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-march='native' -Wall -pedantic")

add_executable(pc-plus-plus src/main.cpp src/producer.cpp src/consumer.cpp src/thread_manager.cpp)
find_package(Threads REQUIRED)
target_link_libraries(pc-plus-plus "${CMAKE_THREAD_LIBS_INIT}")
include_directories(PUBLIC "${CMAKE_SOURCE_DIR}/src/include/")

install(TARGETS pc-plus-plus DESTINATION ${CMAKE_SOURCE_DIR})
