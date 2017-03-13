CC = gcc
CFLAGS = -O0 -g -Wall -lpthread

McDonald:myhead.h queue.c McDonald.c 
	$(CC) $(CFLAGS) -o $@ $^

.PHONY:clean
clean:
	- $(RM) -rf .*.swp *.o McDonald
