all: main

main: main.c
	gcc -std=c99 -O3 -march=native main.c -lm -o main
test:
	time -p ./main
