CC=g++ -std=c++11
SOURCES:=$(wildcard ./src/*.cpp) $(wildcard ../common/src/*.cpp)
OUTPUT=client
ARGS=Name1 3001 1

.PHONY:all
all:build test

.PHONY:build
build:
	$(CC) $(SOURCES) -o $(OUTPUT)

.PHONY:run
test:	
	./$(OUTPUT) $(ARGS)

.PHONY:clean
clean:
	rm -rf ./$(OUTPUT)