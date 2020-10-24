#Makefile for sorted_list library tester
#Author: Jesse Reeve
#Created: 8/18/2020

CC = gcc
CFLAGS = -O0 -g -Wall -Werror
LFLAGS = 

all:  build

build:  main drones darts
	$(CC) $(LFLAGS) -o my-pi main.o drones.o darts.o

main: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

drones: drones.c drones.h
	$(CC) $(CFLAGS) -c -o drones.o drones.c

darts: darts.c darts.h
	$(CC) $(CFLAGS) -c -o darts.o darts.c

clean:
	rm -f *.o my-pi
