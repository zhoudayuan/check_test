CFLAGS=  -g -Wall 
#LDFLAGS= -lpthread
CC=gcc

Src=$(wildcard *.c)
Target=$(patsubst %.c, %, $(Src))






%.o:%.c
	$(CC) $(CFLAGS) -c  $^ -o $@ $(LDFLAGS)

%:%.o
	$(CC) $(CFLAGS) -c  $^ -o $@  $(LDFLAGS)

all: $(Target)

ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS)
endif


clean:
	@echo $(Src)
	@echo $(Target)
	-rm -f $(Target)
