CC     = gcc
CFLAGS = -g -std=c11 -pedantic -Wall

proj1: parsing.o macros.o proj1.o
	${CC} ${CFLAGS} -o proj1 parsing.o macros.o proj1.o

parsing.o: parsing.c parsing.h
macros.o: macros.c macros.h parsing.h
proj1.o: proj1.c proj1.h macros.h parsing.h