#include <stdio.h>

int  main()
{

    FILE *fp_infile;
    char linebuffer[27000];
    int feof_ret, ret;

    if ((fp_infile = fopen("AMBE_Test.ap", "r")) == NULL)
    {
        fprintf(stderr, "Couldn't open input file.\n");
        return -1;
    }

    printf("\n");



#if 0
    printf("\n---------------------\n\n");
    while (fgets(linebuffer, sizeof(linebuffer), fp_infile) != NULL)
    {
//        fputs(linebuffer, stdout);
    }
#else
	
	ret=fread(linebuffer, sizeof(linebuffer[0]), sizeof(linebuffer), fp_infile);
	printf("\nret=%d\n\n", ret);

#endif



    if (((feof_ret = feof(fp_infile)) == 0))
    {
        printf("\n\nfeof_ret=%d, sizeof(linebuffer)=%d\n\n", feof_ret, sizeof(linebuffer));
    }

    printf("\n\nfeof_ret=%d, sizeof(linebuffer)=%d\n\n", feof_ret, sizeof(linebuffer));



    if (fclose(fp_infile) != 0)
    {
        fprintf(stderr, "Couldn't open input file.\n");
        return -2;
    }
    
    
    printf("\n\n");
    return 0;

}