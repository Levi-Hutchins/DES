CC=g++
CFLAGS=-c -Wall -std=c++20
LDFLAGS=
SOURCES=main.cpp DES0.cpp DES1.cpp DES2.cpp DES3.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main
TEST_OBJECTS=$(TEST_SOURCES:.cpp=.o)  # Convert test source files to object files


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_EXECUTABLE): $(TEST_OBJECTS) $(OBJECTS)
	$(CC) $(LDFLAGS) $(TEST_OBJECTS) $(OBJECTS) -o $@


clean:
	rm -rf *.o core $(EXECUTABLE) test/*.o

run-default: $(EXECUTABLE)
	./$(EXECUTABLE) $(ARGS)


encrypt:
	@$(MAKE) all
	@$(MAKE) run-default ARGS="$(ARGS)"
	@$(MAKE) clean

