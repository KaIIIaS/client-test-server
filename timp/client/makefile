.PHONY: all uint16_t clean format zip

CXX=g++
 
CXXFLAGS=-O2 -Wall -DNDEBUG -std=c++17
  
LDFLAGS= -lcrypto++  

EXEC=client
UINT16=$(EXEC)_uint16_t
SOURCE=$(EXEC).cpp

all: uint16_t

uint16_t: $(UINT16)

$(UINT16): $(SOURCE)
	$(CXX) $(SOURCE) -o $(UINT16) $(CXXFLAGS) $(LDFLAGS) -DCODE=16
	
clean:
	rm -f $(EXEC)_*

zip: 
	zip dumb_client.zip $(EXEC)_*
