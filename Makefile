CC = gcc
CFLAGS = -g -Wall -lncurses

build: main.o header.o
	$(CC) main.o header.o $(CFLAGS) -o battleship

main.o: main.c
	$(CC) -c main.c
	
header.o: header.c
	$(CC) -c header.c

clean: battleship *.o
	rm battleship *.o
	rm resume*.txt

keep_history: battleship *.o
	rm battleship *.o

run: battleship
	./battleship harta*.txt
