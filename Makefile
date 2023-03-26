CC=gcc
FLAGS=-std=c11

all: 
	$(CC) src/catavm.c catastring/*.c $(FLAGS) -o cvm

clean:
	rm cvm