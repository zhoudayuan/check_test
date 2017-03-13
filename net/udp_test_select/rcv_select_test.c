#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/select.h>     /* According to POSIX.1-2001 */
#include <sys/time.h>       /* According to earlier standards */
#include <unistd.h>
#include <stdlib.h>
#include "common.h"

#define INF_RECV_MAX   1000


pthread_t g_uCliOutputTid;

//-----CCL-----//

struct sockaddr_in CCLSocketAddr;
static struct sockaddr_in CCSocketAddr;


int CCLSocket;


//------------//


static int TaskCreate();


int INFSocket;
static int  AddrSize = sizeof(struct sockaddr_in);
static void *CliOutputTaskHandle(void *p);







static int TaskCreate()
{
    printf("\n\n[%s:%d]\n\n", __FUNCTION__, __LINE__);

    if (pthread_create(&g_uCliOutputTid, NULL, CliOutputTaskHandle, NULL) != 0)
    {
        printf("\nCreate thread error!\n\n");
        return -1;
    }
    printf("\n\n[%s:%d]\n\n", __FUNCTION__, __LINE__);
    pthread_detach(g_uCliOutputTid);
    return 0;
}




void *CliOutputTaskHandle(void *p)
{

    int i4SelectRet;
    fd_set fdsRecv,fdsEx;
    struct timeval tTimeItval;
    int u4RevLen = 0;
    char auTmpBuff[65535] = {0};
    printf("\n\n[%s:%d]\n\n", __FUNCTION__, __LINE__);

    while (1)
    {
        FD_ZERO(&fdsRecv);
        FD_SET(CCLSocket, &fdsRecv);
        FD_ZERO(&fdsEx);
        FD_SET(CCLSocket, &fdsEx);

        tTimeItval.tv_sec = 10;
        tTimeItval.tv_usec = 0;

        i4SelectRet = select(CCLSocket+1, &fdsRecv, 0, &fdsEx, &tTimeItval);
        if (0 > i4SelectRet)            // errno
        {
            printf("\nselect err:%s\n\n", strerror(errno));
            close(CCLSocket);
            exit(0);
        }
        else if (0 == i4SelectRet)      // time out
        {
            printf("\n\n[%s:%d]\n\n", __FUNCTION__, __LINE__);
        }
        else if (i4SelectRet > 0)       // data for read
        {
            printf("\n\n[%s:%d]\n\n", __FUNCTION__, __LINE__);
        
            if (FD_ISSET(CCLSocket,&fdsRecv))
            {
                u4RevLen = read(CCLSocket, (unsigned char *)auTmpBuff, 65535-1);
                if (0 == u4RevLen)
                {
                    printf("\nINFO:Disconnected form Cli server.\n\n");
                    close(CCLSocket);

                }
                else if(u4RevLen < 0)
                {
                    printf("\nINFO: Read opt error.\n\n");
                    close(CCLSocket);
                }else
                {
                    write(1, auTmpBuff, u4RevLen);
                    printf("\n\n[%s:%d]auTmpBuff=(%s)\n\n", __FUNCTION__, __LINE__, auTmpBuff);
                    auTmpBuff[0]++;
                    CCSocketAddr.sin_family      = AF_INET;
                    CCSocketAddr.sin_port        = htons(SOCK_PORT_CCL_CC_S);
                    CCSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
                    sendto(CCLSocket, auTmpBuff, strlen(auTmpBuff)+1, 0, (struct sockaddr *)(&CCSocketAddr), AddrSize);
                }
            }
            else if (FD_ISSET(CCLSocket, &fdsEx))
            {
                printf("\nselect sock[%d] exception occurs\n\n",CCLSocket);
                close(CCLSocket);

            }
            printf("\n\n[%s:%d]\n\n", __FUNCTION__, __LINE__);
        }
    }
    return NULL;
}











//-------------//-------------//-------------//-------------//
//-------------//-------------//-------------//-------------//
//-------------// Ä£Äâ CCL    //-------------//-------------//
//-------------//-------------//-------------//-------------//
//-------------//-------------//-------------//-------------//

int main ()
{
    CCLSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if (0 > CCLSocket)
    {
        printf("Failed to create signal socket \n");
        return 0;
    }
    printf("\n\n[%s:%d]\n\n", __FUNCTION__, __LINE__);

    bzero(&CCLSocketAddr, sizeof(struct sockaddr_in));
    CCLSocketAddr.sin_family      = AF_INET;
    CCLSocketAddr.sin_port        = htons(SOCK_PORT_CC_CCL_S);
    CCLSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("\n\n[%s:%d]SOCK_PORT_CC_CCL_S=%d\n\n", __FUNCTION__, __LINE__, SOCK_PORT_CC_CCL_S);
    
    if (0 > bind(CCLSocket, (struct sockaddr *)(&CCLSocketAddr), sizeof(struct sockaddr_in)))
    {
        printf("Bind local addr and port error\n");
        printf("\n\n[%s:%d]\n\n", __FUNCTION__, __LINE__);
        perror("bind>");
        close(CCLSocket);
        return -1;
    }
    printf("\n\n[%s:%d]\n\n", __FUNCTION__, __LINE__);

    TaskCreate();
    while (1)
    {
        sleep(5);
        printf("\n\n[%s:%d]\n\n", __FUNCTION__, __LINE__);
    }

    return 0;
}
