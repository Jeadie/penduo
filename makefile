FLAGS = -Wall -pedantic -g

default: main

main.o: main.c $(HEADERS) 
		gcc $(FLAGS) -c main.c -o main.o

main: main.o
		gcc main.o -o main -lm

clean:
		-rm -f main.o
		-rm -f main
