#include <stdio.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>


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



int main ()
{
    int SigSocket;
    int len;
    char buf[]={"123456789\n"};
    int  buf_len = sizeof(buf);
    static struct sockaddr_in CclSigAddr;
    int ToDstLen = sizeof(struct sockaddr_in);

    printf("send the buf=(%s)\n", buf);
    printf("send the len is strlen(buf)=(%d), sizeof(buf)=(%d) \n", strlen(buf), sizeof(buf));

    bzero(&CclSigAddr, sizeof(struct sockaddr_in));
    CclSigAddr.sin_family = AF_INET;
    CclSigAddr.sin_port   = htons(SOCK_PORT_CCL_CC_S);
    // CclSigAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // CclSigAddr.sin_addr.s_addr = inet_addr(INADDR_ANY);
    CclSigAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    SigSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > SigSocket)
    {
        printf("send Failed to create signal socket \n");
        return -1;
    }



    len = sendto(SigSocket, buf, buf_len, 0, (struct sockaddr *)(&CclSigAddr), ToDstLen);

    printf("send len=%d\n", len);


    return 0;
}
