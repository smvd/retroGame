SRCS := $(shell find src/ -type f -name '*.c')
OBJS := $(SRCS:src/%.c=bin/%.o)

FLAGS  := -Wall -Wextra -lraylib

default: run

run: bin/program
	cd bin && ./program

bin/program: $(OBJS)
	gcc $(FLAGS) -o $@ $^

bin/%.o: src/%.c
	gcc $(FLAGS) -o $@ -c $<
