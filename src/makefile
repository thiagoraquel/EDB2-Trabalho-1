CC = clang++
FLAGS = -std=c++17 -o

EXECUTABLES = main

all: $(EXECUTABLES)

%:%.cpp
	$(CC) $(FLAGS) $@ $^

clean:
	rm $(EXECUTABLES)
