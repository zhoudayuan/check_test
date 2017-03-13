


CFLAGS=-O -Wall
CC=gcc


all: rcv send

clean:
	rm -f rcv send


rcv: rcv.c
	
	$(CC) $(CFLAGS) $< -o $@


send: send.c
	$(CC) $(CFLAGS) $< -o $@