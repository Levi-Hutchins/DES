CC=g++
CFLAGS=-c -Wall -std=c++20
LDFLAGS=
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o core $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE) sample_input1/sample.txt

auto:
	@$(MAKE) all
	@$(MAKE) run
	@$(MAKE) clean
	
