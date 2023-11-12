.PHONY: all, clean, format
PROJECT = boost_opts-example
PROJECT2 = boost_opts-example_with_validator
CXX  = g++
CFLAGS = -Wall -std=c++14 -O2 -DNDEBUG
LDFLAGS = -l boost_program_options

all: $(PROJECT) $(PROJECT2)

$(PROJECT): $(PROJECT).cpp
	$(CXX) $(CFLAGS) $<  -o $@ $(LDFLAGS)
	
$(PROJECT2): $(PROJECT2).cpp
	$(CXX) $(CFLAGS) $<  -o $@ $(LDFLAGS)

format:
	astyle $(PROJECT).cpp $(PROJECT2).cpp
	
clean:
	rm -f $(PROJECT) $(PROJECT2) *.orig
