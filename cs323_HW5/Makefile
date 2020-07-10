CC=gcc
CFLAGS=-std=c11 -Wall -pedantic -I.
NAME=abash

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(NAME): abash_process.o abash_main.o abash_parse.o
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: all
all: $(NAME)

.PHONY: clean
clean:
	rm -f abash_process.o abash_main.o $(NAME)
