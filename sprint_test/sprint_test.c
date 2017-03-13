#include <stdio.h>



int main()
{
	char name[50];
	
//	snprintf(name, sizeof(name), "%s", __FUNCTION__);
	snprintf(name, sizeof(name), "%s", __FUNCTION__);
	printf("name=%s\n", name);
	return 0;
	
}