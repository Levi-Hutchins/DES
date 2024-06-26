CC=g++
CFLAGS=-Wall -std=c++20
LDFLAGS=
TEST_EXECUTABLE=compile.test
ALL_SOURCES=$(wildcard ../*.cpp)
MAIN_SOURCES=$(filter-out compile.test.cpp,$(ALL_SOURCES))
MAIN_OBJECTS=$(MAIN_SOURCES:.cpp=.o)
TEST_OBJECT=$(TEST_EXECUTABLE).o

all: main $(TEST_EXECUTABLE)

main: $(MAIN_OBJECTS)
	$(CC) $(LDFLAGS) $(MAIN_OBJECTS) -o $@

$(TEST_EXECUTABLE): $(TEST_OBJECT)
	$(CC) $(LDFLAGS) $(TEST_OBJECT) -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(MAIN_OBJECTS) $(TEST_OBJECT) main $(TEST_EXECUTABLE)

run-main: main
	./main

test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

pipeline-test:
	@$(MAKE) all
	@$(MAKE) test
	@$(MAKE) clean