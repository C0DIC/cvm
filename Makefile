CC=clang
FLAGS=-std=c11 -O3 -Wall

all: 
	$(CC) src/catavm.c catastring/*.c $(FLAGS) -o cvm

clean:
	rm cvm
