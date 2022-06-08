CC=gcc
CFLAGS=-std=gnu17 -ggdb -Wall -Wpedantic -Werror
LIBS=-lm -lSDL2 -lSDL2_image

SRC=$(wildcard src/*.c)
OBJS=$(addprefix obj/, $(SRC:.c=.o))

all:
	mkdir -p obj/src
	$(MAKE) pac

pac: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS)

obj/src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

clean:
	-rm -rf obj a.out

