#include <stdio.h>
#include <unistd.h>



typedef void (*pFunODP)(void);



void ODP_MsDisableIdt(void);
void ODP_MsEnableIdt(void);
void ODP_MsGpsReport(void);
void ODP_NasDisableIdt(void);
void ODP_NasEnableIdt(void);
void ODP_NasGpsReport(void);
void ODP_NasStunIdt(void);
void ODP_NearNodeInfoQuery(void);
void ODP_ShortMessage(void);








int main (int argc, char *argv[])
{
    int i;
    
    
    
//    getopt(argc, argv, );
    
    
    
    pFunODP  aFunODP[] = {
        ODP_MsDisableIdt,
        ODP_MsEnableIdt,
        ODP_MsGpsReport,
        ODP_NasDisableIdt,
        ODP_NasEnableIdt,
        ODP_NasGpsReport,
        ODP_NasStunIdt,
        ODP_NearNodeInfoQuery,
        ODP_ShortMessage
    };
    
    
    
    for (i = 0; i < 9; i++)
    {
            aFunODP[i]();

    }


}




void ODP_MsDisableIdt()
{
    printf("1\r\n");
}


void ODP_MsEnableIdt()
{
printf("2\r\n");
}
void ODP_MsGpsReport()
{
    printf("3\r\n");
}

void ODP_NasDisableIdt()
{
printf("4\r\n");
}
void ODP_NasEnableIdt()
{
    printf("5\r\n");
}
void ODP_NasGpsReport()
{
printf("6\r\n");
}
void ODP_NasStunIdt()
{
    printf("5\r\n");
}
void ODP_NearNodeInfoQuery()
{
printf("6\r\n");
}
void ODP_ShortMessage()
{
printf("7\r\n");
}

