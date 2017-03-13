#include <stdio.h>
#include <strings.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <sys/select.h>     /* According to POSIX.1-2001 */
#include <sys/time.h>       /* According to earlier standards */

#include <pthread.h>



#define INF_RECV_MAX   1000
static struct sockaddr_in CCSigAddr;
static void *CliOutputTaskHandle(void *p);
static int TaskCreate();
int  AddrSize = sizeof(struct sockaddr_in);
pthread_t g_uCliOutputTid;


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




static int TaskCreate()
{
    if (pthread_create(&g_uCliOutputTid, NULL, CliOutputTaskHandle, NULL) != 0)
    {
        printf("\nCreate thread error!\n\n");
        return -1;
    }
    pthread_detach(g_uCliOutputTid);
    return 0;
}


 int s_hsocket;

void *CliOutputTaskHandle(void *p)
{
    
    int i4SelectRet;
    fd_set fdsRecv,fdsEx;
    struct timeval tTimeItval;
//    int u4RevLen = 0;
//    char auTmpBuff[65535] = {0};

    printf("\n\n[%s:%d]\n\n", __FUNCTION__, __LINE__);
    while (1)
    {
    	
        FD_ZERO(&fdsRecv);
        FD_SET(s_hsocket,&fdsRecv);
        FD_ZERO(&fdsEx);
        FD_SET(s_hsocket,&fdsEx);

    	
		tTimeItval.tv_sec = 10;
        tTimeItval.tv_usec = 0;

        i4SelectRet = select(s_hsocket+1,&fdsRecv,0,&fdsEx,&tTimeItval);
        if (0 > i4SelectRet)
        {
        	

        }
        else if (0 == i4SelectRet)
        {
        	
        }
        else if (i4SelectRet > 0)
        {
        	if(FD_ISSET(s_hsocket, &fdsRecv))
        	{
        			
			}
        }
    }
    return NULL;
}



int main ()
{

    int SigSocket;

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

    TaskCreate();
    close(SigSocket);
    
    return 0;
}
