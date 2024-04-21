CC=g++
CFLAGS=-c -Wall -std=c++20
LDFLAGS=
SOURCES=test.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o core $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

auto:
	@$(MAKE) all
	@$(MAKE) run
	@$(MAKE) clean
	
