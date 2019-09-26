Producer-Consumer++
======
C++ version of [Producer-Consumer](https://github.com/PierceGriffiths/Producer-Consumer/).
As of 25 September 2019, [Producer-Consumer](https://github.com/PierceGriffiths/Producer-Consumer/) has switched from pthreads to C11 threads, making Producer-Consumer++ redundant. This project will receive little to no maintenance from this point forwards, because I've learned to hate C++ and OOP in general while working on it.

## What's the same
* Portability
* Argument ordering and (theoretical) maximum width
* Control flow
* Output format
* Log file format

## What's different - the good
* All structs from the original project have been rewritten as classes
* Producers and consumers have had their critical sections moved to dedicated functions in order to provide abstraction of their implementation details
* Underlying classes are highly reusable with little to no modification

## What's different - the bad (as of 20 April 2019 with GCC 8.3 on Debian Testing x86)
* Executable size (with `make release`)
	* Producer-Consumer produces a 23112 byte (22.57 KiB) binary
    * Producer-Consumer++ produces a 53640 byte (52.38 KiB) binary
* Average compile time (`make cleanall && time make release` was run four times for each project)
    * Producer-Consumer has an average compile time of 0.338 seconds
    * Producer-Consumer++ has an average compile time of 2.6305 seconds
* Producer-Consumer++ is, on average, about 40% slower than Producer-Consumer when both versions are given identical arguments
