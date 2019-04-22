CXX := g++
SDIR := ./src
IDIR := $(SDIR)/include
CXXFLAGS := -march='native' -std=c++14 -pedantic -I$(IDIR) -Wall
ODIR := $(SDIR)/obj
ifneq ($(OS),Windows_NT)
    LDLIBS := -pthread
endif #if not compiling for Windows

ifeq ($(MAKECMDGOALS),$(filter $(MAKECMDGOALS),release pc-plus-plus))
    CXXFLAGS := $(CXXFLAGS) -O3
else ifeq ($(MAKECMDGOALS),$(filter $(MAKECMDGOALS),debug pc-plus-plus-debug))
    CXXFLAGS := $(CXXFLAGS) -Og -g -pg
    ODIR := $(ODIR)/debug
endif #if compiling debug version

DEPS := $(wildcard $(IDIR)/*.hpp)

_OBJ := $(patsubst %.cpp, %.o, $(notdir $(wildcard $(SDIR)/*.cpp)))
OBJ := $(patsubst %, $(ODIR)/%, $(_OBJ))

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
	rm -f $(ODIR)/*.o $(ODIR)/debug/*.o *~ core $(IDIR)/*~

cleanall: cleanobj
	rm -f pc-plus-plus pc-plus-plus-debug
