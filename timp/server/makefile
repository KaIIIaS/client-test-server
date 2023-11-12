.PHONY: all clean format

CXX=g++
CXXFLAGS=-O2 -Wall -DNDEBUG -std=c++17
LDFLAGS= -lcrypto++ 
PROJECT = server
SOURCES := $(wildcard *.cpp)
HEADERS := $(wildcard *.h)
OBJECTS := $(SOURCES:%.cpp=%.o)

all : $(PROJECT)

$(PROJECT) : $(OBJECTS)
ifneq (,$(findstring B,$(firstword $(MAKEFLAGS))))
	$(CXX) -static  $^ $(LDFLAGS) -o $@
else
	$(CXX)  $^ $(LDFLAGS) -o $@
endif

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJECTS) : $(HEADERS)

format:
	astyle *.cpp *.h
	
clean:
	rm -f $(PROJECT) *.o *.orig
