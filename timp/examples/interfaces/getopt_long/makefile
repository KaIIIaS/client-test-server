.PHONY: all, clean, format
PROJECT = getopt_long-example
CXX  = g++
CFLAGS = -Wall -std=c++14 -O2 -DNDEBUG

all: $(PROJECT)

$(PROJECT): $(PROJECT).cpp
	$(CXX) $(CFLAGS) $< -o $@

format:
	astyle $(PROJECT).cpp
	
clean:
	rm -f $(PROJECT) *.orig
