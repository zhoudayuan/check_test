#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
//#include "sigtest.h"

#define  TRUE                       1
#define  FALSE                      0
#define  PPT_ON                     1
#define  PPT_OFF                    0


#define  ARG_CMD_FEATURE            1       // 功能测试
#define  ARG_CMD_PRESSURE           2       // 压力测试

#define  ARG_FEATURE_TYPE_VOICE     1       // 功能中的语音测试
#define  ARG_FEATURE_TYPE_DATA      2       // 功能中的数据测试

#define  ARG_VOICE_MODEL_AMBE       1       // 语音中的AMBE测试
#define  ARG_VOICE_MODEL_NVOC       2       // 语音中的NVOC测试

#define  ARG_VOICE_PACKET_MAX       2000    // 功能测试语音包的最大长度
#define  ARG_VOICE_PACKET_MIN       1       // 功能测试语音包的最小长度
#define  ARG_VOICE_PACKET_DEFAULT   -1      // 功能测试语音包的默认长度

#define  POCKET_BUFF_LEN            5       // 语音包的字节长度
#define  POCKET_BYTE_MAX            1000    // 语音包最大包数
#define  POCKET_BYTE_MIN            200     // 语音包最小包数

#define  DATA_ITEM_BUFF_LEN         2       // 字节包字节长度
#define  OK                         0
#define  NO_INPUT                   1
#define  TOO_LONG                   2

#define  VOICE_FRAME_LEN            (27*1000)      // 语音帧长度

#define  AMBE_FORMAT                1
#define  NVOC_FORMAT                2

#define  AMBE_FILE_NAME             "AMBE_Test.ap"
#define  NVOC_FILE_NAME             "NVOC_Test.ap"
#define  SEND_FRAME_MAX             10      // 字节数=27*SEND_FRAME_MAX , AMBE_Test.ap=27K

#define GET_SUCCEED         0
#define TOO_LONG_ERR        -1
#define NO_INPUT_ERR        -2
#define NOT_DIGIT_ERR       -3
#define CR_ERR              -4



 /**
 * @def  RECV_DLL_PORT
 * @brief 本地发送DLL 端口
 */
#define CC_SEND_VOICE_PORT          (10011)// (6017)
/**
 * @def  SEND_CCL_PORT
 * @brief  本地接收DLL端口
 */
#define  CC_SEND_SIG_PORT           (10013) //(6018)
/**
 * @def  RTP_MAX_DATA_SIZE
 * @brief  中心语音负载长度
 */
#define   RTP_MAX_DATA_SIZE           28

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// 加入到工程后删除
#include "typedef.h"
typedef  enum  _SMS_DATA_CALL_TYPE
{
    //短消息
    MESSAGE_PRIVATE_CALL    =0x00,    //单呼
    MESSAGE_GROUP_CALL      =0x01,    //组呼
    STATUS_PRIVATE_CALL     =0x02,    //状态单呼
    STATUS_GROUP_CALL       =0x03,    //状态组呼

    //命令
    STUN_REQ_MS             =0x07,    //遥晕MS
    STUN_REQ_NAS            =0x08,    //遥晕NAS
    KILL_REQ_NAS            =0x09,    //遥毙NAS
    GPS_REPOTR_MS           =0x0a,    //拉GPS
    GPS_REPORT_NAS          =0x0b,
    REVIVE_REQ_NAS          =0x0c,
    REVIVE_REQ_MS           =0x0d,
    NEGHR_QUERY             =0x0e,     //邻点查询

    //响应
    NEGHR_QUERY_ACK         =0x20,     //邻点查询相应   sms_send 格式回复
    GPS_REPROT_MS_ACK       =0x21,     //相应拉MS  GPS       CCL 先回ACK标识收到命令           然后  sms_send 格式回复数据信息
    GPS_REPROT_NAS_ACK      =0x22,     //相应拉NAS  GPS      CCL 先回ACK标识收到命令     sms_send 格式回复
    STUN_REQ_MS_ACK         =0x23,
    STUN_REQ_NAS_ACK        =0x24,
    KILL_REQ_NAS_ACK        =0X25,
    REVIVE_REQ_NAS_ACK      =0x26,     //激活NAS
    REVIVE_REQ_MS_ACK       =0x27,     //激活MS

    //告警
    DISCON_ALARM            =0x30,     //激活NAS    //DISCON_ALARM            =0x13,                       //断连告警
    DISCON_ALARM_CLEAR      =0x31,     //断连告警清除
    //增加临点上报

}SMS_DATA_CALL_TYPE;


/**
 * @struct MSG_DATA_DL_T
 * @brief 中心下行有效数据结构体
 */
typedef struct _MSG_DATA_DL_T
{
    UINT32 call_id;
    UINT32 voice_id;
    UINT8 source_stat;
    UINT8 sms_type;
    UINT16 valid_length;
    UINT8 sms_format;
    UINT8 sender_num[30];
    UINT8 receiver_num[30];
    UINT8 sms_data[200];
    UINT8 reserved[3];
} MSG_DATA_DL_T;


/**
 * @struct CC_CCL_DL_T
 * @brief 中心下行到呼叫控制层结构体
 */
typedef struct _CC_CCL_DL_T
{
    UINT16 Head;
    UINT16 Type;
    UINT16 Datalength;
    UINT16 Source_ID;
    UINT16 Dest_ID;
    UINT8 Source_Type;
    UINT8 Sig_Version;
    UINT8 Path_Num;
    UINT8 Ex_head[11];
    UINT32 Check_Val;
    MSG_DATA_DL_T MsgData;

} CC_CCL_DL_T;





//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@




typedef struct _CACHE_T
{
    char *pBuff;
    int len;
} CACHE_T;



typedef struct _MAIN_ARG_T
{

    int cmd;        // 功能or压力
    int cmd_ch;     // '-c'
    int type;       // 语音or数据
    int type_ch;    // '-t'
    int format;     // 语音格式: 1-ambe 2-nvoc
    int item;       // 数据测试项
    int len;        // 语音包长度
    int clock;
    int source;
    int dest;
    int groupID;
} MAIN_ARG_T;

typedef  enum  _DATA_TEST_ITEM
{
    ITEM_MS_REMOTE_KILL  = 0,
    ITEM_MS_ENABLE       = 1,
    ITEM_MS_GPS_REPORT   = 2,
    ITEM_NAS_DISABLE     = 3,
    ITEM_NAS_ENABLE      = 4,
    ITEM_NAS_GPS_REPORT  = 5,
    ITEM_NAS_STUN        = 6,
    ITEM_NEIGHBOR_QUERY  = 7,
    ITEM_SHORT_MESSAGE   = 8,
    ITEM_MAX             = 9
} DATA_TEST_ITEM;

/**
 * @struct _CENTER_CMD_SHAREHEADER
 * @brief  接入单元与中心命令交互共用头
 */
typedef struct  _CENTER_CMD_SHARE_HEAD
{
    unsigned short SigHead;          // 标识AC接口信令头标志(0x13ec)
    unsigned short SigType;          // 信令类型 标识AC接口PTT命令信令（0x000a）
    unsigned short Datalength;       // 有效数据长度 标识信令有效数据长度（100）
    unsigned short SourceID;         // 源单元号 标识AC接口发送单元ID值
    unsigned short DestID;           // 目的单元号 标识AC接口接收单元ID值
    unsigned char  SourceType;       // 源单元设备类型 标识发送单元设备类型（制式）
    unsigned char  SigVersion;       // 信令版本号 标识该信令适合版本号0x02(该信令与0x01版本不兼容)
    unsigned char  PathNum;          // 通道号码 标识该信令属于该单元的通道号码，若不区分通道，则填全F
    unsigned char  Exhead[11];       // 扩展头 扩展头中心无须关心参数
    unsigned int   CheckVal;         // 校验值 标识信令校验结果(01版本 与02版本 目前都填写全F，标示无校验方式)
} CENTER_CMD_SHARE_HEAD ;





/**
 * @struct  _PTT_CMD
 * @brief  PTT-ON  PTT-OFF  命令结构体
 */
typedef struct _PTT_CMD
{
    CENTER_CMD_SHARE_HEAD  SharedHead;
    unsigned int  CallId;             // 控制面ID 标识一次呼叫的控制面ID
    unsigned int  VoiceId;            // 业务面ID 标识一次呼叫的业务面ID
    unsigned char PttStat;            // PTT信令状态  标识PTT信令状态（请求CMD_ON=0x06、释放CMD_OFF=0x07）
    unsigned char Reserved;           // 字节对齐占位中心无须关心参数
    unsigned char TalkingNum[30];     // 标识当次通话PTT的讲话方号码
    unsigned char CallingNum[30];     // 标识当次呼叫的主叫号码
    unsigned char CalledNum[30];      // 标识当次呼叫的被叫号码（组号、个号）
} PTT_CMD;


/**
 * @struct  _Rtp_Ex_Head
 * @brief RTP 扩展头
 */
typedef struct _Rtp_Ex_Head
{
     unsigned short profile_head;
     unsigned short exhead_length;       // 32bit number == 4 bytes number
     unsigned int   voice_id;
     unsigned int   gps;
     unsigned char  codec_flag;
     unsigned char  frame_flag;
     unsigned char  path_num;            // show which path voice 0xff indicates invalid value, "0" indicates the first path
     unsigned char  pack_rate;           // packet rate "20" or "60" milliseconds
     unsigned char  encrypt_flag;
     unsigned char  encode_flag;
     unsigned char  encode_frame;
     unsigned char  enconde_rate;
} Rtp_Ex_Head;



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




typedef int (*pITEM_TEST_F)(MAIN_ARG_T *ptMainArg);

typedef struct _DATA_ITEM_FUN_T
{
    DATA_TEST_ITEM item;
    pITEM_TEST_F pFun;
} DATA_ITEM_FUN_T;


// 变量
static int s_tCenterVoiceSockfd;                // 中心语音套接字
static struct sockaddr_in s_tCenterVoiceAddr;
static int s_tCenterSigSockfd ;                 // 中心信令套接字
static struct sockaddr_in s_tCenterSigAddr;
//static int OPD_packetnum;                     // 发送下行语音包数目
// -------
static int  gMessage_len;       // 短信长度
static int  gMessage_ID;        // 短信ID
static char gSrcDev = 0;
static char gDstDev = 0;



// 函数
static int MS_Remote_Kill_test(MAIN_ARG_T* ptMainArg);      //0
static int Ms_Enable_test(MAIN_ARG_T* ptMainArg);           //1
static int Ms_Gps_Report_test(MAIN_ARG_T* ptMainArg);       //2
static int Nas_Disable_test(MAIN_ARG_T* ptMainArg);         //3
static int Nas_Enable_test(MAIN_ARG_T* ptMainArg);          //4
static int Nas_Gps_Report_test(MAIN_ARG_T* ptMainArg);      //5
static int Nas_Stun_test(MAIN_ARG_T* ptMainArg);            //6
static int Neighbor_Query_test(MAIN_ARG_T* ptMainArg);      //7
static int Short_Message_test(MAIN_ARG_T* ptMainArg);       //8
//----
static int digit_get(char *prmpt, char *buff, int sz, int *pDigit);
static int getLine (char *prmpt, char *buff, int sz);


//
static void ODP_ShortMessage(void);








DATA_ITEM_FUN_T atDataItemFun[] = {
    {ITEM_MS_REMOTE_KILL , MS_Remote_Kill_test},            // 0
    {ITEM_MS_ENABLE      , Ms_Enable_test},                 // 1
    {ITEM_MS_GPS_REPORT  , Ms_Gps_Report_test},             // 2
    {ITEM_NAS_DISABLE    , Nas_Disable_test},               // 3
    {ITEM_NAS_ENABLE     , Nas_Enable_test},                // 4
    {ITEM_NAS_GPS_REPORT , Nas_Gps_Report_test},            // 5
    {ITEM_NAS_STUN       , Nas_Stun_test},                  // 6
    {ITEM_NEIGHBOR_QUERY , Neighbor_Query_test},            // 7
    {ITEM_SHORT_MESSAGE  , Short_Message_test},             // 8
    {ITEM_MAX            , NULL}                            // 9
};



int get_src_dst_id(const char *name, char *SrcDev, char *DstDev)
{
    char buff[10];
    int digit;
    int ret;

    while(1)
    {
        printf("\n\n%s\nPlease Inpur Srcdev Value (0 < Srcdev < 31):\n\n", name);
        if ((ret = digit_get("Enter Src Value>", buff, 3, &digit)) == GET_SUCCEED)
        {
            if (digit <= 31 && digit >= 0)
            {
                *SrcDev = digit;
                printf("\nSrcDev=%d\n\n", *SrcDev);
                break;
            }
            printf("\nThe Value Range Err\n");
        }
    }

    while(1)
    {
        printf("\n\nPlease Inpur DstDev Value (0 < Srcdev < 31):\n\n");
        if ((ret = digit_get("Enter Dst Value>", buff, 3, &digit)) == GET_SUCCEED)
        {
            if (digit <= 31 && digit >= 0)
            {
                *DstDev = digit;
                printf("\nSrcDev=%d\n\n", *DstDev);
                break;
            }
            printf("\nThe Value Range Err\n");
        }
    }
    return 0;
}


/**
 * @brief   下行NAS 激活指示发送
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
static void ODP_NasEnableIdt(void)
{
    printf("Nas enable idte download \n");
    CC_CCL_DL_T CcSMessage;

    CcSMessage.Head = htons(0x13ec);
    CcSMessage.Type = 0x000e;
    CcSMessage.Datalength = sizeof(MSG_DATA_DL_T);
    CcSMessage.Source_ID = 0xffff;
    CcSMessage.Dest_ID = 0xffff;
    CcSMessage.Source_Type = 0xff;
    CcSMessage.Sig_Version = 0x02;
    CcSMessage.Path_Num = 0xff;
    CcSMessage.Check_Val = 0xffffffff;

    memset(CcSMessage.Ex_head, 0xff, 11);

    CcSMessage.MsgData.call_id = 0x0001;
    CcSMessage.MsgData.voice_id = 0xffff;
    CcSMessage.MsgData.source_stat = 0;
    CcSMessage.MsgData.sms_type = REVIVE_REQ_NAS;
    CcSMessage.MsgData.valid_length = 0;
    CcSMessage.MsgData.sms_format = 0x01;
//    CcSMessage.MsgData.sender_num[0] = SrcDev;
    CcSMessage.MsgData.sender_num[0] = gSrcDev;
    CcSMessage.MsgData.sender_num[1] = 0x00;
    CcSMessage.MsgData.sender_num[2] = 0x00;
//    CcSMessage.MsgData.receiver_num[0] = DstDev;
    CcSMessage.MsgData.receiver_num[0] = gDstDev;
    CcSMessage.MsgData.receiver_num[1] = 0x00;
    CcSMessage.MsgData.receiver_num[2] = 0x00;

    sendto(SigSocket, &CcSMessage, sizeof(CC_CCL_DL_T), 0, (struct sockaddr *)(&CclSigAddr), UDPSize);
}




/**
 * @brief   下行短消息发送
 * @author  陈禹良
 * @since   trunk.00001
 * @bug
 */
#if 0

void ODP_ShortMessage(void)
{
    printf("short message download \n");
    CC_CCL_DL_T CcSMessage;

    CcSMessage.Head = htons(0x13ec);
    CcSMessage.Type = 0x000e;
    CcSMessage.Datalength = sizeof(MSG_DATA_DL_T);
    CcSMessage.Source_ID = 0xffff;
    CcSMessage.Dest_ID = 0xffff;
    CcSMessage.Source_Type = 0xff;
    CcSMessage.Sig_Version = 0x02;
    CcSMessage.Path_Num = 0xff;
    CcSMessage.Check_Val = 0xffffffff;

    memset(CcSMessage.Ex_head, 0xff, 11);

    CcSMessage.MsgData.call_id = 0x0001;
    CcSMessage.MsgData.voice_id = 0xffff;
    CcSMessage.MsgData.source_stat = 0;
//    CcSMessage.MsgData.sms_type = MESSAGE_GROUP_CALL;
    CcSMessage.MsgData.sms_type = gMessage_ID;
//    CcSMessage.MsgData.valid_length = 18;
    CcSMessage.MsgData.valid_length = gMessage_len;
    CcSMessage.MsgData.sms_format = 0x01;
    CcSMessage.MsgData.sender_num[0] = 0x00;
    CcSMessage.MsgData.sender_num[1] = 0x00;
    CcSMessage.MsgData.sender_num[2] = 0x01;
    CcSMessage.MsgData.receiver_num[0] = 0x00;
    CcSMessage.MsgData.receiver_num[1] = 0x03;
    CcSMessage.MsgData.receiver_num[2] = 0xe8;
    CcSMessage.MsgData.sms_data[0] = 'h';
    CcSMessage.MsgData.sms_data[1] = 0x00;
    CcSMessage.MsgData.sms_data[2] = 'y';
    CcSMessage.MsgData.sms_data[3] = 0x00;
    CcSMessage.MsgData.sms_data[4] = 't';
    CcSMessage.MsgData.sms_data[5] = 0x00;
    CcSMessage.MsgData.sms_data[6] = 'e';
    CcSMessage.MsgData.sms_data[7] = 0x00;
    CcSMessage.MsgData.sms_data[8] = 'r';
    CcSMessage.MsgData.sms_data[9] = 0x00;
    CcSMessage.MsgData.sms_data[10] = 'a';
    CcSMessage.MsgData.sms_data[11] = 0x00;
    CcSMessage.MsgData.sms_data[12] = '1';
    CcSMessage.MsgData.sms_data[13] = 0x00;
    CcSMessage.MsgData.sms_data[14] = '2';
    CcSMessage.MsgData.sms_data[15] = 0x00;
    CcSMessage.MsgData.sms_data[16] = '3';
    CcSMessage.MsgData.sms_data[17] = 0x00;

    sendto(SigSocket, &CcSMessage, sizeof(CC_CCL_DL_T), 0, (struct sockaddr *)(&CclSigAddr), UDPSize);


}
#endif


int MS_Remote_Kill_test(MAIN_ARG_T* ptMainArg)              // 0
{ printf("\n[%s:%d]\n\n", __FUNCTION__, __LINE__);}
int Ms_Enable_test(MAIN_ARG_T* ptMainArg)                   // 1
{  printf("\n[%s:%d]\n\n", __FUNCTION__, __LINE__); }
int Ms_Gps_Report_test(MAIN_ARG_T* ptMainArg)               // 2
{  printf("\n[%s:%d]\n\n", __FUNCTION__, __LINE__); }


int Nas_Disable_test(MAIN_ARG_T* ptMainArg)                 // 3
{  


    char name[50];
//    char buff[10];
//    int digit;
//    int ret;
    char SrcDev;
    char DstDev;
    
    snprintf(name, sizeof(name), "%s", __FUNCTION__);
    get_src_dst_id(name, &SrcDev, &DstDev);



#if 0
    while(1)
    {
        printf("\n\nNas Disable Test\n Please Inpur Srcdev Value (0 < Srcdev < 31):\n\n");
        if ((ret = digit_get("Enter Src Value>", buff, 3, &digit)) == GET_SUCCEED)
        {
//            printf("\n\nget digit=%d\n\n", digit);
            if (digit <= 31 && digit >= 0)
            {
                SrcDev = digit;
                printf("\nSrcDev=%d\n\n", SrcDev);
                break;
            }
            printf("\nThe Value Range Err\n");
        }
    }

    while(1)
    {
        printf("\n\nPlease Inpur DstDev Value (0 < Srcdev < 31):\n\n");
        if ((ret = digit_get("Enter Dst Value>", buff, 3, &digit)) == GET_SUCCEED)
        {
//            printf("\n\nget digit=%d\n\n", digit);
            if (digit <= 31 && digit >= 0)
            {
                DstDev = digit;
                printf("\nSrcDev=%d\n\n", DstDev);
                break;
            }
            printf("\nThe Value Range Err\n");
        }
    }
#endif
    gSrcDev = SrcDev;
    gDstDev = DstDev;
    printf("\ng@@SrcDev=%d\n\n", gSrcDev);
    printf("\ng@@SrcDev=%d\n\n", gDstDev);

	
}


int Nas_Enable_test(MAIN_ARG_T* ptMainArg)                  // 4
{
    char buff[10];
    int digit;
    int ret;
    char SrcDev = 0;
    char DstDev = 0;

    while(1)
    {
        printf("\n\n%s\nPlease Inpur Srcdev Value (0 < Srcdev < 31):\n\n", __FUNCTION__);
        if ((ret = digit_get("Enter Src Value>", buff, 3, &digit)) == GET_SUCCEED)
        {
//            printf("\n\nget digit=%d\n\n", digit);
            if (digit <= 31 && digit >= 0)
            {
                SrcDev = digit;
                printf("\nSrcDev=%d\n\n", SrcDev);
                break;
            }
            printf("\nThe Value Range Err\n");
        }
    }

    while(1)
    {
        printf("\n\nPlease Inpur DstDev Value (0 < Srcdev < 31):\n\n");
        if ((ret = digit_get("Enter Dst Value>", buff, 3, &digit)) == GET_SUCCEED)
        {
//            printf("\n\nget digit=%d\n\n", digit);
            if (digit <= 31 && digit >= 0)
            {
                DstDev = digit;
                printf("\nSrcDev=%d\n\n", DstDev);
                break;
            }
            printf("\nThe Value Range Err\n");
        }
    }

    gSrcDev = SrcDev;
    gDstDev = DstDev;

    printf("\n[%s:%d]\n\n", __FUNCTION__, __LINE__);

}
int Nas_Gps_Report_test(MAIN_ARG_T* ptMainArg)              // 5
{  printf("\n[%s:%d]\n\n", __FUNCTION__, __LINE__); }
int Nas_Stun_test(MAIN_ARG_T* ptMainArg)                    // 6
{  printf("\n[%s:%d]\n\n", __FUNCTION__, __LINE__); }
int Neighbor_Query_test(MAIN_ARG_T* ptMainArg)              // 7
{  printf("\n[%s:%d]\n\n", __FUNCTION__, __LINE__); }



static int Short_Message_test(MAIN_ARG_T* ptMainArg)
{
    char buff[10];
    int digit;
    int ret;
    int Message_ID;
    int Message_len;
    // 获取组ID
    while (1)
    {
        printf("\n\n1) select call type:\n\
        \n\t1 - Private Call\
        \n\t2 - Group Call\n\
        \nPlease press the number to select: ");
        if ((ret = digit_get("Enter type>", buff, 2, &digit)) == GET_SUCCEED)
        {
//            printf("\n\nget digit=%d\n\n", digit);
            if (digit < 3)
            {
                Message_ID =((--digit==MESSAGE_GROUP_CALL) ? MESSAGE_GROUP_CALL : MESSAGE_PRIVATE_CALL);
                printf("\nMessage_ID=%d\n\n", Message_ID);
                break;
            }
        }
    }

    // 获取短信长度
    while (1)
    {
        printf("\n\nHow Many Message Bytes(0~100) You Need? Please Input\n\n");
        if ((ret = digit_get("Enter Bytes>", buff, 4, &digit)) == GET_SUCCEED)
        {
            printf("\n\nGet Message Bytes=(%d)\n\n", digit);
            if (digit >=0 && digit <=100)
            {
                Message_len=digit;
                printf("\nMessage_len=%d\n\n", Message_len);
                break;
            }
        }
    }


    // 调用测试程序
   gMessage_len = Message_len;
   gMessage_ID =  Message_ID;
//   ODP_ShortMessage();



    return 0;
}




static int digit_get(char *prmpt, char *buff, int sz, int *pDigit)
{
    char *pbuff = buff;
    int ret, ch;
    ret = getLine(prmpt, buff, sz);
    if (ret == TOO_LONG)
    {
        printf("\n\nInput Number Is Too Long, Please Try Again\n\n");
        return TOO_LONG_ERR;
    }
    else if (ret == NO_INPUT)
    {
        printf ("\n\nNo Input, Please Try Again\n\n");
        return NO_INPUT_ERR;
    }
    else if (ret == OK)
    {
        while ((ch = *pbuff++) != '\0')
        {
            if (isdigit(ch) == 0)
            {
                printf("\n\nInput Is Not Digit, Please Try Again\n\n");
                return NOT_DIGIT_ERR;
            }
        }

        if (buff[0] == '\0')
        {
            printf("\n\nInput Data Is Cr, Try Again\n\n");
            return CR_ERR;
        }

        *pDigit = atoi(buff);
//        printf("\n\nYou Get Digit=%d\n\n", *pDigit);
    }
    return  GET_SUCCEED;
}

void delay(unsigned long msec)
{
    struct timeval tv;
    tv.tv_sec  = msec / 1000;
    tv.tv_usec = (msec % 1000) * 1000;

    switch (select(0, NULL, NULL, NULL, &tv))
    {
        case -1:
        {
            printf("delay Error!\n");
            break;
        }
        case 0:
        {
            break;
        }
        default:
        {
            printf("delay default\n");
            break;
        }
    }
}


// -1 表示 失败
unsigned long get_file_size(const char *path)
{
    unsigned long filesize = -1;
    struct stat statbuff;

    if (stat(path, &statbuff) < 0)
    {
        return filesize;
    }
    else
    {
        filesize = statbuff.st_size;
    }
    return filesize;
}


int sh_getch(void)
{
    int cr;
    struct termios nts, ots;

    if (tcgetattr(0, &ots) < 0)          // 得到当前终端(0表示标准输入)的设置
        return EOF;

    nts = ots;
    cfmakeraw(&nts);                     // 设置终端为Raw原始模式，该模式下所有的输入数据以字节为单位被处理
    if (tcsetattr(0, TCSANOW, &nts) < 0) // 设置上更改之后的设置
        return EOF;

    cr = getchar();
    if (tcsetattr(0, TCSANOW, &ots) < 0) // 设置还原成老的模式
        return EOF;

    return cr;
}


static int getLine (char *prmpt, char *buff, int sz)
{
    int ch, extra;

    if (prmpt != NULL)
    {
        printf ("%s", prmpt);
        fflush (stdout);
    }

    if (fgets (buff, sz, stdin) == NULL)
    {
        printf("NO_INPUT\n");
        return NO_INPUT;
    }

//    printf("[%s:%d]buff=(%s), sz=(%d), strlen(buff)=(%d)\n", __FUNCTION__, __LINE__, buff, sz, strlen(buff));

    if (buff[strlen(buff)-1] != '\n')
    {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
        {
            extra = 1;
        }
        return (extra == 1) ? TOO_LONG : OK;
    }

    buff[strlen(buff)-1] = '\0';
    return OK;
}


int MainFeatureVoiceCtrTest(MAIN_ARG_T *ptMainArg)
{

    if ((ptMainArg->format == ARG_VOICE_MODEL_AMBE) || (ptMainArg->format == ARG_VOICE_MODEL_NVOC))
    {
        if ((ptMainArg->len > ARG_VOICE_PACKET_MAX) && (ptMainArg->len > ARG_VOICE_PACKET_MIN))
        {
//          MainFeatureVoice
        }
        else if (ptMainArg->len == ARG_VOICE_PACKET_DEFAULT)  // 默认
        {

        }
        else
        {
            printf("voice packet err , please try again\n");
            exit(0);
        }
    }
    else
    {

        printf("format err, please try again\n");
        exit(0);
    }
    return TRUE;
}




void ShowHelp(void)
{
    printf("\
    \n\
    \n\
    Usage:\n\
    NasTest -c [PARAM_1] -t [PARAM_2]\n\
    NasTest -h\n\
    \n\
    explain:\n\
        -h -- help\n\
        -c -- command, parameter is 1 or 2, 1 mean feature test, 2 mean pressure test \n\
        -t -- type,    parameter is 1 or 2, 1 mean voice test,   2 mean data test \n\
    \n\
    Examples:\n\
    NasTest -h              # help\n\
    NasTest -c 1 -t 1       # feature test, voice test\n\
    NasTest -c 1 -t 2       # feature test, data  test\n\
    NasTest -c 2            # pressure test \n\
    .....\n\
    .....\n\
    \n\
    \n");
}



void ODP_FeatureVoiceSocketInit()
{
    // 中心语音初始化
    s_tCenterVoiceSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (s_tCenterVoiceSockfd <0)
    {
          printf("[%s:%d] creat centervoicefd err\n", __FUNCTION__, __LINE__);
          exit(0);
    }
    bzero(&s_tCenterVoiceAddr, sizeof(s_tCenterVoiceAddr));
    s_tCenterVoiceAddr.sin_family = AF_INET;
    s_tCenterVoiceAddr.sin_addr.s_addr =htonl(INADDR_ANY);  // htonl(INADDR_ANY);  //inet_addr("127.0.0.1");//这里不一样
    s_tCenterVoiceAddr.sin_port = htons(CC_SEND_VOICE_PORT);

    // 中心语音信令初始化
    s_tCenterSigSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if( s_tCenterSigSockfd<0)
    {
          printf("[%s:%d] creat centersigfd err\n", __FUNCTION__, __LINE__);
          exit(0);
    }
    bzero(&s_tCenterSigAddr, sizeof(s_tCenterSigAddr));
    s_tCenterSigAddr.sin_family = AF_INET;
    s_tCenterSigAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_tCenterSigAddr.sin_port = htons(CC_SEND_SIG_PORT);
    printf("\n[%s:%d] \n\n", __FUNCTION__, __LINE__);
}

void ODP_ppt_Off(void)
{
    unsigned char SndBuf[512];
    PTT_CMD *ptSndBuf = (PTT_CMD *)SndBuf;
    unsigned short u2datasendlen;
    struct timeval    sigstop;
    struct timezone   tz;
    memset(ptSndBuf, 0, sizeof(PTT_CMD));
    ptSndBuf->SharedHead.SigHead = 0xec13;
    ptSndBuf->PttStat = 0x07;               //PTT mingling
    ptSndBuf->SharedHead.SigType = 0x000a;  //PTT_ON
    ptSndBuf->CallingNum[0] = 0;
    ptSndBuf->CallingNum[1] = 0;
    ptSndBuf->CallingNum[2] = 143;
    ptSndBuf->CalledNum[0]  = 0;
    ptSndBuf->CalledNum[1]  = 0x03;
    ptSndBuf->CalledNum[2]  = 0xe8;
    u2datasendlen = sendto( s_tCenterSigSockfd,  ptSndBuf, sizeof(PTT_CMD), 0, (struct sockaddr *)&s_tCenterSigAddr, sizeof(s_tCenterSigAddr)); // @@数字什么意思
    printf("PTT OFF sendlen =%d\n",u2datasendlen);
//    gettimeofday(&sigstop,&tz);
//    printf("ptt off time: %d s, %d us\n", (int)sigstop.tv_sec, (int)sigstop.tv_usec);
}





void ODP_ppt_on(void)
{
    unsigned char SndBuf[512];
    PTT_CMD *ptSndBuf = (PTT_CMD *)SndBuf;
    unsigned short u2datasendlen;
    struct timeval  sigstart;
    struct timezone tz;
    memset(ptSndBuf, 0, sizeof(ptSndBuf));
    ptSndBuf->SharedHead.SigHead = 0xec13;
    ptSndBuf->PttStat = 0x06;              //PTT mingling 0X06
    ptSndBuf->SharedHead.SigType = 0x000a; //PTT_ON
    ptSndBuf->CallingNum[0] = 0;
    ptSndBuf->CallingNum[1] = 0;
    ptSndBuf->CallingNum[2] = 143;
    ptSndBuf->CalledNum[0]  = 0;
    ptSndBuf->CalledNum[1]  = 0x03;
    ptSndBuf->CalledNum[2]  = 0xe8;
    u2datasendlen = sendto(s_tCenterSigSockfd, ptSndBuf, sizeof(PTT_CMD), 0, (struct sockaddr *)&s_tCenterSigAddr, sizeof(s_tCenterSigAddr));
    printf("PTT on sendlen =%d\n",u2datasendlen);
    gettimeofday(&sigstart, &tz);
    printf("ptt on time: %d s, %d us \n",  (int)sigstart.tv_sec, (int)sigstart.tv_usec);
}





void ODP_Voice_Send(int len, int format)
{
    FILE *fp;
    int ret;
    char *pVoiceFileName = NULL;
    int SendLen = len;  // 20~1000
    int index = 0;
    CACHE_T tCache;
    CACHE_T *pCache = &tCache;
    CENTER_VOICE_DATA tCenterVoiceData;
    CENTER_VOICE_DATA *pSendBuff = &tCenterVoiceData;
    memset(pSendBuff, 0, sizeof(CENTER_VOICE_DATA));

    // 获取声音文件名
    pVoiceFileName = ((format == AMBE_FORMAT) ? AMBE_FILE_NAME : NVOC_FILE_NAME);
    printf("pVoiceFile=%s\n", pVoiceFileName);


    // 判断该文件是否存在
    if ((ret = access(pVoiceFileName, F_OK)) < 0)
    {
        printf("%s is not exist(ret=%d)\n", pVoiceFileName, ret );
        exit(0);
    }

    if ((fp = fopen(pVoiceFileName, "rb")) == NULL)
    {
        printf("CC open file err\n");
        exit(0);
    }

    // 获取该文件字节数
    pCache->len = get_file_size(pVoiceFileName);
//    printf("pCache->len=%d\n", pCache->len);

    // 获取堆内存
    if ((pCache->pBuff = (char *)malloc(pCache->len)) == NULL)
    {
        perror("malloc");
        fclose(fp);
        exit(0);
    }

    // 将文件读取到内存中
    ret = fread(pCache->pBuff, sizeof(pCache->pBuff[0]), pCache->len, fp);
    if (ret != pCache->len)
    {
        perror("fread: ");
        fclose(fp);
        free(pCache->pBuff);
        exit(0);
    }

#if 0
    ret = feof(fp);
    printf("feof: ret=%d\n", ret);
    // 判断是否全部读出
    if (!feof(fp))
    {
        printf("Error reading from input file.\n");
        free(pCache->pBuff);
        exit(0);
    }
#endif

    if (fclose(fp) != 0)
    {
        printf("Error closing input file.\n");
        free(pCache->pBuff);
        exit(0);
    }

    while (1)
    {
        memcpy(pSendBuff->Payload, pCache->pBuff + 27*index++, 27);
        sendto(s_tCenterVoiceSockfd, pSendBuff, sizeof(CENTER_VOICE_DATA), 0, (struct sockaddr *)&s_tCenterVoiceAddr, sizeof(s_tCenterVoiceAddr));
        delay(35);

        // 内存越界判断
        if (index*27 < pCache->len)
        {
            // 数据帧，一帧27 bytes
            if (index == SendLen)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    printf("send:(%d)bytes, (%d)frames to ccl over\n", index*27, index);
    free(pCache->pBuff);
}



int FeatureVoiceParaGet(MAIN_ARG_T *ptMainArg)
{
    int format;
    int ch, len;
    int i = 0;
    char buff[POCKET_BUFF_LEN];

    while (1)
    {
        printf("\n\n1) The voice format as follow\n\
        1 - ambe\n\
        2 - nvoc\n\
        \nPlease press the number to select: ");

        while ((isdigit (ch = getc(stdin))) && ((ch == '1') || (ch == '2')))
        {

            if (ch == '1')
            {
                while((ch=getchar())!='\n' && ch != EOF);   // 清除 stdin 缓存, 具体是清除 '\n'
                printf("\nYou choose: ambe\n");
                ptMainArg->format = 1;
                goto Voice_Len_Select;
            }
            else if (ch == '2')
            {
                while((ch=getchar())!='\n' && ch != EOF);   // 清除 stdin 缓存, 具体是清除 '\n'
                printf("\nYou choose: nvoc\n");
                ptMainArg->format = 2;
                goto Voice_Len_Select;
            }
        }
        while((ch=getchar())!='\n' && ch != EOF);   // 清除 stdin 缓存, 具体是清除 '\n'
        printf("\n\nsorry, the number error, please try again, press any key to continue>");
        sh_getch();
    }

Voice_Len_Select:

    while (1)
    {
        i = 0;
        memset(buff, 0, sizeof(buff));
        printf("\n2) Input the packet len (range: 100~500)\n\
               \nthe number is: ");

        if (fgets(buff, sizeof(buff), stdin) == NULL)
        {
            printf("fgets wrong, please try again\n");
            return -1;
        }

        if (buff[strlen(buff)-1] != '\n')
        {
            // 输入数据太长，以至于无'\n'
            // 所以，需要清除 stdin 缓冲
            while (((ch = getchar()) != '\n') && (ch != EOF));   // 清除 stdin中的 '\n'
        }
        else
        {
            // \n' 替换为 '\0'
            buff[strlen(buff)-1] = '\0';
        }

        // 判断输入字符是否为数字
        while ((ch = buff[i++]) != '\0')
        {
            if (isdigit(ch) == 0)
            {
                // 非数字
                printf("input data is not digit, please try again \n");
                goto Voice_Len_Select;
            }
        }

        // 格式化字符，将其转换为整形
        sscanf(buff, "%d", &len);
        if (len >= POCKET_BYTE_MIN && len <= POCKET_BYTE_MAX)
        {
            ptMainArg->len = len;
            break;
        }
        else if (len > POCKET_BYTE_MAX )
        {
            printf("\nThe packet len you input is too large,\
            \nSo, set it as the default maximum (%d)\n\n", POCKET_BYTE_MAX);
            ptMainArg->len = POCKET_BYTE_MAX;
            break;
        }
        else if (len < POCKET_BYTE_MIN)
        {
            printf("\nThe packet len you input is too little,\
            \nSo, set it as the default minimum (%d)\n\n", POCKET_BYTE_MIN);
            ptMainArg->len = POCKET_BYTE_MIN;
            break;
        }
        else
        {
            printf("[%s:%d] len=(%d)(%x) err \n", __FUNCTION__, __LINE__, len, len);
            goto Voice_Len_Select;
        }
    }
    return 0;
}




int FeatureVoiceStart(MAIN_ARG_T *ptMainArg)
{
    ODP_FeatureVoiceSocketInit();
    ODP_ppt_on();
    ODP_Voice_Send(ptMainArg->len, ptMainArg->format);
    ODP_ppt_Off();
    return 0;
}



int FeatureDataParaGet(MAIN_ARG_T *ptMainArg)
{
    char buff[DATA_ITEM_BUFF_LEN];
    char *pbuff = buff;
    int ret;
    int ch;
    int item;

Data_Item_Select:

    while (1)
    {
        printf("\n\nData test items as follows\n\n\
        0 - MS Remote Kill\n\
        1 - Ms Enable\n\
        2 - Ms Gps Report\n\
        3 - Nas Disable\n\
        4 - Nas Enable\n\
        5 - Nas Gps Report\n\
        6 - Nas Stun\n\
        7 - Neighbor Query\n\
        8 - Short Message\n\
        \nPlease press the 0 ~ 8 to select\n");

        ret = getLine("Enter number>", buff, sizeof(buff));
        if (ret == TOO_LONG)
        {
            printf("\nThe input number is too long, you can only enter one digit\
            \nplease try again, press any key to continue>");
            sh_getch();
            goto Data_Item_Select;
        }
        else if (ret == NO_INPUT)
        {
            printf ("\nNo input, please try again, press any key to continue\n");
            sh_getch();
            goto Data_Item_Select;

        }
        else if (ret == OK)
        {
            pbuff = buff;
            while ((ch = *pbuff++) != '\0')
            {
                if (isdigit(ch) == 0)
                {
                    printf("\n\ninput is not digit, try again, press any key to continue\n\n");
                    sh_getch();
                    goto Data_Item_Select;
                }
            }

            if (buff[0] == '\0')
            {
                printf("\n\nInput data is CR, try again, press any key to continue\n\n");
                sh_getch();
                goto Data_Item_Select;
            }

            pbuff = buff;
//          printf("\nYou input is (%s)\n\n", pbuff );

            // 判断范围
            if ((item = atoi(buff)) >= ITEM_MAX)
            {
                printf("\nThere is no item=%d, try again, press any key to continue\n\n", item);
                goto Data_Item_Select;
            }
            printf("\nYou Choose Item (%d) To Test\n\n",  item);
            ptMainArg->item = item;
            return item;        // 返回item;
        }
    }
}


int FeatureDataSingleStart(MAIN_ARG_T *ptMainArg)
{
    int item = ptMainArg->item;
    int index;
    printf("\n[%s:%d] item=(%d)\n", __FUNCTION__, __LINE__, item);
//    printf("\n[%s:%d] sizeof(atDataItemFun)/sizeof(MAIN_ARG_T)=(%d)\n", __FUNCTION__, __LINE__, sizeof(atDataItemFun)/sizeof(DATA_ITEM_FUN_T));

//    for (index = 0; index < sizeof(atDataItemFun)/sizeof(DATA_ITEM_FUN_T) - 1; index++)
    for (index = 0; atDataItemFun[index].item != ITEM_MAX; index++)
    {
//        printf("\n[%s:%d] atDataItemFun[index].item=(%d)\n", __FUNCTION__, __LINE__, atDataItemFun[index].item);
        if (item == atDataItemFun[index].item)
        {
            (atDataItemFun[index].pFun)(ptMainArg);
            break;
        }
    }
    exit(0);
//    return 0;
}


int TestStart(MAIN_ARG_T *ptMainArg)
{
    if (ptMainArg->cmd == ARG_CMD_FEATURE)              // 是否为功能测试
    {
        if (ptMainArg->type == ARG_FEATURE_TYPE_VOICE)  // 是否为功能测试中的语音测试
        {
            // 语音测试, 单次测试
            FeatureVoiceParaGet(ptMainArg);             // 获取语音参数，格式和长度
            FeatureVoiceStart(ptMainArg);               // 启动语音测试，

        }
        else if (ptMainArg->type == ARG_FEATURE_TYPE_DATA)
        {
            FeatureDataParaGet(ptMainArg);              //
            FeatureDataSingleStart(ptMainArg);
        }
        else                                            // err
        {
            printf("[%s:%d] the type(-t) err, it's not 1 or 2 \n",  __FUNCTION__, __LINE__);
            exit(0);
        }
    }
    else if (ptMainArg->cmd == ARG_CMD_PRESSURE)        // 压力测试
    {

    }
    else
    {
        printf("[%s:%d] the -c parameter is err\n", __FUNCTION__, __LINE__);
        ShowHelp();
        exit(0);

    }
    return TRUE;
}






int MainArgvParse(int argc, char *argv[], MAIN_ARG_T *ptMainArg)
{
    int opt;
    int err_flag = 0;
    char *optstring = "c:t:h";


    while ((opt = getopt(argc, argv, optstring)) != -1)
    {
#if 0
        // 检查函数返回值
        printf("opt    = %c\n",    opt);
        printf("optarg = %s\n", optarg);
        printf("optind = %d\n", optind);
        printf("argv[optind - 1] = %s\n",  argv[optind - 1]);
#endif

        switch (opt)
        {
            case 'c':
            {

                ptMainArg->cmd_ch = opt;
                ptMainArg->cmd = atoi(optarg);
                break;
            }
            case 't':
            {
                ptMainArg->type_ch = opt;
                ptMainArg->type = atoi(optarg);
                break;
            }
            case 'h':
            {
                ShowHelp();
                exit(0);
                break;
            }
            default:
            {
                exit(0);
                break;
            }
        }
    }

    // check (-c) options
    if (ptMainArg->cmd_ch != 'c')
    {
        printf("\noptions cmd (-c) err, there is no -c\n\n");
        err_flag = 1;
    }
    else
    {
        if ((ptMainArg->cmd != 1) && (ptMainArg->cmd != 2))
        {
            printf("\nnoptions cmd(-c) data info(%d) err, it's not 1 or 2\n\n", ptMainArg->cmd);
            err_flag = 1;
        }
    }



    // check (-t) options
    if (ptMainArg->type_ch != 't' )
    {
        printf("\noptions cmd (-t) err, there is no -t\n\n");
        err_flag = 1;
    }
    else
    {
        if ((ptMainArg->type != 1) && (ptMainArg->type != 2))
        {
            printf("\noptions cmd(-t) data info(%d) err, it's not 1 or 2\n\n", ptMainArg->type);
            err_flag = 1;
        }
    }

    if (err_flag == 1)
    {
        printf("\nPlease input again:  ./NasTest -c [PARAM_1] -t [PARAM_2]\n\n");
        exit(0);
    }
    return TRUE;
}





// -c==cmd       功能/压力测试命令，可选参数: 1-功能测试，2-压力测试
// -t==type      语音/数据类型      可选参数:
// -f=format     语音测试编码模式
// -n==item      数据测试编号
// -l==len       包长度
// -k==clock     时间毫秒
// -s==source    原地址
// -d==dest      目的地址
// -g==groupID   组ID
// -o==IPD/OPD   方向，上下行
int main(int argc, char *argv[])
{
    int i;
    MAIN_ARG_T tMainArg;
    MAIN_ARG_T *ptMainArg = &tMainArg;
    int *p = (int *)&tMainArg;
    int len = sizeof(tMainArg)/sizeof(int);

    // 对MAIN_ARG_T每个成员初始化 -1
    for (i = 0; i < len; i++)
    {
        p[i] = -1;
    }

    // 输入时候不带参数显示帮助
    if (argc == 1)
    {
        ShowHelp();
        exit(0);
    }

    MainArgvParse(argc, argv, ptMainArg);   // 解析入参argv , 并检查参数内容
    TestStart(ptMainArg);                   // 启动测试
    return 0;
}
