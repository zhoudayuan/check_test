CFLAGS =  -O -Wall
CC = gcc


EXECUTABLES = INIT_test \
				getopt \
				tcpserver 	\
				fopen_test \
				inet_aton_test \
				gets_test \
				HexToStr_test\
				GetSmsData


%o:%c
	$(CC) $(CFLAGS) $< -o $@

all	  : $(EXECUTABLES)

clean :
	-rm $(EXECUTABLES)

