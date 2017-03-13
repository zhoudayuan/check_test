#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	char *optstr="if:lr";
	int opt;
	
	while ((opt = getopt(argc, argv, optstr) ) != -1)
	{
		printf("(%c) (%d)\n", opt, opt);
	}
	return 0;
	
}


/*


*/