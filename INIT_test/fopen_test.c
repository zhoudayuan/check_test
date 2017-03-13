
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>





int test_fopen(int argc, char *argv[])
{
    FILE *pLogTxtFd = NULL;
    
    pLogTxtFd = fopen("./LOG.txt","r+");

    if(NULL == pLogTxtFd)
    {
        creat("./LOG.txt", S_IRUSR | S_IWUSR);
        pLogTxtFd = fopen("./LOG.txt","r+");
        if(NULL == pLogTxtFd)
        {
            printf("[LOG]fopen Log.txt fail!!!\n");
            perror("fopen>");
            exit(1);
        }   
    }
    fseek(pLogTxtFd,0,SEEK_END);
    return 0;
}


#if 0
if ((fd = open(path, O_WRONLY)) < 0) {
if (errno == ENOENT) {
if ((fd = creat(path, mode)) < 0)
err_sys("creat error");
}else {
err_sys("open error");
}
}

#endif





int main(int argc, char *argv[])
{
    test_fopen(argc, argv);
    return 0;
}




