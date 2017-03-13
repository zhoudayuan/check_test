#include <stdio.h>
#include <strings.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>

#define INF_RECV_MAX   1000


typedef enum {
    SOCK_PORT_CC_V_SEND  = 10000,
    SOCK_PORT_CC_V_RECV  = 10001,
    SOCK_PORT_CC_S_SEND  = 10002,
    SOCK_PORT_CC_S_RECV  = 10003,

    SOCK_PORT_CCL_CC_V   = 10010,
    SOCK_PORT_CC_CCL_V   = 10011,
    SOCK_PORT_CCL_CC_S   = 10012,
    SOCK_PORT_CC_CCL_S   = 10013,
    SOCK_PORT_MGRA_CC    = 10014,
    SOCK_PORT_CC_MGRA    = 10015,

    SOCK_PORT_CCL_DLL    = 10020,
    SOCK_PORT_DLL_CCL    = 10021,

    SOCK_PORT_DLL_ADP    = 10030,
    SOCK_PORT_ADP_DLL    = 10031,

    SOCK_PORT_MGR_ADP    = 10040,
    SOCK_PORT_ADP_MGRA   = 10041,
    SOCK_PORT_MGRA_MGRH  = 10042,
    SOCK_PORT_MGRH_MGRA  = 10043,
    SOCK_PORT_ADP_MGRH   = 10044,
    SOCK_PORT_CLI_INPUT = 10050,
    SOCK_PORT_LOG_PRINT = 10051
}SOCK_PORT;



static struct sockaddr_in CCSigAddr;
int  AddrSize = sizeof(struct sockaddr_in);


int main ()
{

    int SigSocket, len;
    char RecvBuf[INF_RECV_MAX];
    SigSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if (0 > SigSocket)
    {
        printf("Failed to create signal socket \n");
        return 0;
    }

    bzero(&CCSigAddr, sizeof(struct sockaddr_in));
    CCSigAddr.sin_family      = AF_INET;
    CCSigAddr.sin_port        = htons(SOCK_PORT_CCL_CC_S);
    CCSigAddr.sin_addr.s_addr = INADDR_ANY;

    if (0 > bind(SigSocket, (struct sockaddr *)(&CCSigAddr), sizeof(struct sockaddr_in)))
    {
        printf("Bind sigsocketError \n");
        return -1;
    }

    while (1)
    {
        len = recvfrom(SigSocket, RecvBuf, INF_RECV_MAX, 0, (struct sockaddr *)(&CCSigAddr), &AddrSize);
        printf("[%s:%d] rcv buf is (%s)\n", __FUNCTION__, __LINE__, RecvBuf);
        printf("[%s:%d] rcv len=(%d)\n", __FUNCTION__, __LINE__,  len);

    }


    close(SigSocket);
    return 0;
}
