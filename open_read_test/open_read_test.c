#include <stdio.h>
#include <stdlib.h>

//#include "cc_ccl_dl.h"



/**
 * @def  RTP_MAX_DATA_SIZE
 * @brief  中心语音负载长度
 */
#define   RTP_MAX_DATA_SIZE  28

/**
 * @struct  _Rtp_Fix_Head
 * @brief RTP 固定头
 */

typedef struct   _Rtp_Fix_Head
{
    unsigned char   version:2;
    unsigned char   padding_flag:1;
    unsigned char   ex_head_flag:1;
    unsigned char   csrc_count:4;
    unsigned char   marker:1;
    unsigned char   payload_type:7;
    unsigned short  sequence_num;
    unsigned int    time_stamp;
    unsigned int    ssrc;
} Rtp_Fix_Head;



/**
 * @struct  _Rtp_Ex_Head
 * @brief RTP 扩展头
 */
typedef struct _Rtp_Ex_Head
{
    unsigned short profile_head;
    unsigned short exhead_length;      // 32bit number == 4 bytes number
     unsigned int  voice_id;
     unsigned int  gps;
     unsigned char codec_flag;
     unsigned char frame_flag;
     unsigned char path_num;            // show which path voice 0xff indicates invalid value, "0" indicates the first path
     unsigned char pack_rate;           // packet rate "20" or "60" milliseconds
     unsigned char encrypt_flag;
     unsigned char encode_flag;
     unsigned char encode_frame;
     unsigned char enconde_rate;
} Rtp_Ex_Head;


/**
 * @struct   _CENTER_VOICE_DATA
 * @brief 中心语音数据包格式
 */
typedef struct _CENTER_VOICE_DATA
{
    Rtp_Fix_Head    Rtp_Head;
    Rtp_Ex_Head     Rtp_Ex;
    unsigned char   Payload[RTP_MAX_DATA_SIZE];
    unsigned char   Reserve_Data[5];
} CENTER_VOICE_DATA;





int main(int argc, char *argv[])
{
    FILE *fp;
    int readlen;
    int i = 0;
    int j;
    int k;
    unsigned char  ucSentBuf[1024];
    CENTER_VOICE_DATA *ptSendBuf = (CENTER_VOICE_DATA *)ucSentBuf;

    printf("[%s:%d] Hello The Fucking World argc=%d\n", __FUNCTION__, __LINE__, argc);

    while (argc-- > 1)
    {
        printf("[%s:%d] argv=%s\n", __FUNCTION__, __LINE__,*++argv);

    }


//    fp = fopen ("AMBE_Test.ap", "r");
    fp = fopen ("zhou.ap", "r");
    if (fp == NULL)
    {
        printf("CC testsorce open file err\n");
        exit(0);
    }

    for (j = 0; j < 6;j++)
    {
        readlen = fread(ptSendBuf->Payload, sizeof(char), 20, fp);


#if 0
        if (readlen != 27)
        {
            printf("[CENTER]  read file err  len=%d\n", readlen);
            fclose(fp);
            exit(0) ;
        }
#endif


        printf("[%s:%d] readlen=%d\n", __FUNCTION__, __LINE__, readlen);

        for (i = 0; i < readlen; i++)
        {
            printf("%#x(%c) ", ptSendBuf->Payload[i], ptSendBuf->Payload[i]);
        }
        getchar();
        printf("\n");
    }

    return 0;
}

