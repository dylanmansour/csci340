CC=gcc
CFLAGS=-c

all: hw3

hw3: shell.o hw3.o
	$(CC) shell.o hw3.o -o hw3

shell.o: shell.c
	$(CC) $(CFLAGS) shell.c

hw3.o:	hw3.c
	$(CC) $(CFLAGS) hw3.c

clean:
	/bin/rm -f hw3 *.o *.gz

run:
	./hw3

tarball:
	# put your tar command here
	tar -cvzf anderson.tar.gz *
