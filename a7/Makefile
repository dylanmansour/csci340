CC=gcc
CFLAGS=-c -O2 -Wall -g

all: hw7

hw7: mem.o main.o
	$(CC) mem.o main.o -o hw7

mem.o: mem.c
	$(CC) $(CFLAGS) mem.c

main.o: main.c
	$(CC) $(CFLAGS) main.c

clean:
	/bin/rm -f hw7 *.o *.gz

run:
	./hw7

tarball:
	# put your tar command here
	tar -cvzf anderson.tar.gz *

