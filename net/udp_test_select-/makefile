all: rcv_select_test send


clean:
	rm   rcv_select_test send



rcv_select_test: rcv_select_test.c
	gcc -o rcv_select_test rcv_select_test.c  -lpthread
	
	
send: send.c
	gcc -o send send.c  -lpthread
