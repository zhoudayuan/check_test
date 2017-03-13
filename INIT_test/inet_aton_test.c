#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



int test_inet_aton(int argc, char *argv[])
{
	char *str = NULL;
	struct sockaddr_in vchu_addr;
	inet_aton("127.0.0.2\n",&vchu_addr.sin_addr);
	str = inet_ntoa(vchu_addr.sin_addr);
	printf("\n[%s:%d]str=%s\n\n", __FUNCTION__, __LINE__, str);
	printf("\n[%s:%d]str=%s\n\n", __FUNCTION__, __LINE__, str);
	return 0;
}





int main(int argc, char *argv[])
{
	test_inet_aton(argc, argv);
	
	
	return 0;
}




