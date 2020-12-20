 
CXX=g++
CXXFLAGS=-std=c++11 -O3 -Wall -pedantic
LDFLAGS=-std=c++11

EXEC=saha
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
DEP=$(wildcard *.hpp)


all: $(EXEC)

$(EXEC): $(OBJ) $(DEP)
	$(CXX) $^ $(LDFLAGS)  -o $(EXEC)
	@tar -a -cf Saha_v2.tar ../Saha_v2
	@mv -f Saha_v2.tar ../
	cat *.cpp *.hpp *.tpp | wc -l

%.o: %.cpp %.hpp 
	$(CXX) $(CXXFLAGS) -c -o $@  $<

clean:
	rm -dfr *.o  $(EXEC)
