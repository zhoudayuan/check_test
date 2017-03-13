#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INIT()			( program_name = \
						strrchr( argv[ 0 ], '/' ) ) ? \
						program_name++ : \
						( program_name = argv[ 0 ] )

char *program_name;

int main(int argc, char *argv[])
{
	INIT();
	printf("program_name=%s\n\n", program_name);
	return 0;
}

