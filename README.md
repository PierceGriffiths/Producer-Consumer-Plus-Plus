Producer-Consumer++
======
C++ version of [Producer-Consumer](https://github.com/PierceGriffiths/Producer-Consumer/).

## What's the same
* Argument ordering and (theoretical) maximum width
* Control flow
* Output format
* Log file format

## What's different - the good
* Producer-Consumer++ is intended to work on all common platforms, and uses the standard C++ thread library
* All structs from the original project have been rewritten as classes
* Producers and consumers have had their critical sections moved to dedicated functions in order to provide abstraction of their implementation details

## What's different - the bad (as of 15 April 2019 with GCC 8.3 on Debian Testing x86)
* Executable size (with `make release`)
    * Producer-Consumer produces a 23112 byte (22.57 KiB) binary
    * Producer-Consumer++ produces a 52280 byte (51.05 KiB) binary
* Average compile time (`make cleanall && time make release` was run four times for each project)
    * Producer-Consumer has an average compile time of 0.338 seconds
    * Producer-Consumer++ has an average compile time of 2.6305 seconds
* Average execution time (4 executions performed for each set of parameters)
    * With 10 producer threads, 5 consumer threads, a buffer capacity of 7 items, and a target of 50 items:
	* Producer-Consumer has an average execution time of 0.0085 seconds
	* Producer-Consumer++ has an average execution time of 0.01125 seconds
    * With 50 producer threads, 40 consumer threads, a buffer capacity of 45 items, and a target of 2000 items:
	* Producer-Consumer has an average execution time of 0.06675 seconds
	* Producer-Consumer++ has an average execution time of 0.09475 seconds
    * With 100 producer threads, 80 consumer threads, a buffer capacity of 90 items, and a target of 10000 items:
	* Producer-Consumer has an average execution time of 0.3385 seconds
	* Producer-Consumer++ has an average execution time of 0.46125 seconds
