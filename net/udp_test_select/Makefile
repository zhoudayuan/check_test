CFLAGS=-O -Wall
LDFLAGS= -lpthread
CC=gcc


all: rcv_select_test send

clean:
	rm -f rcv_select_test send


rcv_select_test: rcv_select_test.c 
	
	$(CC) $(CFLAGS) $< -o $@  $(LDFLAGS)


send: send.c 
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)