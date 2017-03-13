


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int test_gets(int argc, char *argv[])
{
	int len;
    char buffer[8];
    printf("\n[%s:%d] check gets ...\n\n", __FUNCTION__, __LINE__);
    while ( gets( buffer ) != NULL )
    {
    	
    	printf("\n[%s:%d]buffer=(%s) len=(%d)\n\n", __FUNCTION__, __LINE__, buffer, len=strlen(buffer));
    	
    }

    



    return 0;
}

int test_fgets(int argc, char *argv[])
{
	int len;
    char buffer[8];
    printf("\n[%s:%d] check fgets ...\n\n", __FUNCTION__, __LINE__);
    while (fgets( buffer, sizeof(buffer), stdin )!= NULL)
    {
          	printf("\n[%s:%d]buffer=(%s) len=(%d)\n\n", __FUNCTION__, __LINE__, buffer, len=strlen(buffer));
    }

      	printf("\n[%s:%d]buffer=(%s) len=(%d)\n\n", __FUNCTION__, __LINE__, buffer, len=strlen(buffer));
    return 0;
}



int main(int argc, char *argv[])
{
//	test_gets(argc, argv);
    test_fgets(argc, argv);
    return 0;
}




