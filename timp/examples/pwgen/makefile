.PHONY: all clean format
CXX = g++
CXXFLAGS = -Wall -std=c++17 -O2 -DNDEBUG
LDFLAGS = -l boost_program_options
PROJECT = pwgen
SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:%.cpp=%.o)

all : $(PROJECT)

$(PROJECT) : $(OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $@

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

main.o : generator.h interface.h 
interface.o generator.o: generator.h constants.h 


format:
	astyle *.cpp *.h
	 
clean :
	rm -f $(PROJECT) *.o *.orig
