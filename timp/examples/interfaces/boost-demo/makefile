.PHONY: all, clean, format
PROJECT = boost_demo
CXX  = g++
CFLAGS = -Wall -std=c++14 -O2 -DNDEBUG
LDFLAGS = -l boost_program_options

all: $(PROJECT) $(PROJECT2)

$(PROJECT): $(PROJECT).cpp
	$(CXX) $(CFLAGS) $<  -o $@ $(LDFLAGS)
	
format:
	astyle $(PROJECT).cpp 
	
clean:
	rm -f $(PROJECT) *.orig
