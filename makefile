CXX=g++
CXXFLAGS= -march='native' -std=c++14 -pedantic -I$(IDIR) -Wall
SDIR=./src
IDIR=$(SDIR)/include
ODIR=$(SDIR)/obj
ifneq ($(OS),Windows_NT)
    LDLIBS=-pthread
endif #if not compiling for Windows

ifeq ($(MAKECMDGOALS),$(filter $(MAKECMDGOALS),release pc-plus-plus))
    CXXFLAGS += -O3
else ifeq ($(MAKECMDGOALS),$(filter $(MAKECMDGOALS),debug pc-plus-plus-debug))
    CXXFLAGS += -Og -g -pg
    ODIR=$(SDIR)/obj/debug
endif #if compiling debug version



_DEPS = macro_defs.hpp exceptions.hpp queue.hpp thread_args.hpp threaded_functions.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = thread_args.o producer.o consumer.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	@if [ ! -d "$(ODIR)" ]; then	\
	    mkdir -p $(ODIR);           \
	fi;                             #create appropriate object directory if it doesn't exist
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: release debug cleanobj cleanall

release: pc-plus-plus

debug: pc-plus-plus-debug

pc-plus-plus: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

pc-plus-plus-debug: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

cleanobj:
	rm -f $(ODIR)/*.o $(ODIR)/debug/*.o *~ core $(INCDIR)/*~

cleanall: cleanobj
	rm -f pc-plus-plus pc-plus-plus-debug
