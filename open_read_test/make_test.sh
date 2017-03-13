#!/bin/sh

#gcc_path=`pwd`/tools/gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux/bin
#gcc_path=`pwd`/tools/arm-cortexa9-linux-gnueabihf/gcc-4.7.3-glibc-2.16.0-binutils-2.22-kernel-3.6-sanitized/bin
#cd $Protocol_Path
#Application
#$gcc_path/arm-linux-gnueabihf-g++ -O2 -Wall -DLINUX -o sap sap.c dll.c dll_algrthm.c dll_idp.c dll_odp.c dll_interleave.c ccl_dl.c ccl_ul.c ccl_thread.c ccl_fun.c -I ../inc -lpthread
#$gcc_path/arm-cortexa9-linux-gnueabihf-g++ -O2 -Wall -DLINUX -o NasTest sigtest.c -I ../inc  -lpthread


#É¾³ý
if [ -e test_check ]
then
	rm test_check
	echo "~~rm~ok~~\n"
fi


#±àÒë
gcc -Wall -o test_check open_read_test.c 

#½á¹û
if [ -e test_check ]
then
	echo "\n~~generate~ok~~\n"
	echo "~~start~~\n\n"
	./test_check
fi

echo "\n"