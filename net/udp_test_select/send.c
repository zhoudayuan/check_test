#include <stdio.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include "common.h"





static struct sockaddr_in CCLSocketAddr;
static struct sockaddr_in CCSocketAddr;
static int  AddrSize = sizeof(struct sockaddr_in);




#define  INF_RECV_MAX   1000

//----------// Ä£ÄâCC //----------//
int CCSocket;

int main ()
{

    int len;
    char buf[]={"123456789\n"};
    char RecvBuf[INF_RECV_MAX];
    int  buf_len = sizeof(buf);
    
    int ToDstLen = sizeof(struct sockaddr_in);

    printf("send the buf=(%s)\n", buf);
    printf("send the len is strlen(buf)=(%d), sizeof(buf)=(%d) \n", strlen(buf), sizeof(buf));



    CCSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > CCSocket)
    {
        printf("send Failed to create signal socket \n");
        return -1;
    }

    //------SND-----    
    bzero(&CCLSocketAddr, sizeof(struct sockaddr_in));
    CCLSocketAddr.sin_family = AF_INET;
    CCLSocketAddr.sin_port   = htons(SOCK_PORT_CC_CCL_S);
    CCLSocketAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //------RCV-----
    bzero(&CCSocketAddr, sizeof(struct sockaddr_in));
    CCSocketAddr.sin_family = AF_INET;
    CCSocketAddr.sin_port   = htons(SOCK_PORT_CCL_CC_S);
    CCSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);


    if (0 > bind(CCSocket, (struct sockaddr *)(&CCSocketAddr), sizeof(struct sockaddr_in)))
    {
        printf("Bind local addr and port error\n");
        printf("\n\n[%s:%d]\n\n", __FUNCTION__, __LINE__);
        perror("bind>");
        close(CCSocket);
        return -1;
    }


    len = sendto(CCSocket, buf, buf_len, 0, (struct sockaddr *)(&CCLSocketAddr), ToDstLen);

    while (1)
    {
        len = recvfrom(CCSocket, RecvBuf, sizeof(RecvBuf), 0, (struct sockaddr *)(&CCSocketAddr), &AddrSize);
        printf("[%s:%d] rcv buf is (%s)\n", __FUNCTION__, __LINE__, RecvBuf);
        printf("[%s:%d] rcv len=(%d)\n", __FUNCTION__, __LINE__,  len);
    }
    printf("send len=%d\n", len);
    return 0;
}
