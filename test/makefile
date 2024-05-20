CXX=g++
CXXFLAGS=-Wall -std=c++20
LDFLAGS=
SOURCES=../DES0.cpp ../DES1.cpp ../DES2.cpp ../DES3.cpp des.tests.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=des_tests

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean run

clean:
	rm -rf *.o $(EXECUTABLE) ../*.o

run: $(EXECUTABLE)
	./$(EXECUTABLE)

test:
	@$(MAKE) run
	@$(MAKE) clean

