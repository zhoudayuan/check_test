#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define POCKET_BYTE_LEN  5
#define POCKET_BYTE_MAX  500
#define POCKET_BYTE_MIN  100

#define  OK                         0
#define  NO_INPUT                   1
#define  TOO_LONG                   2




#define   RTP_MAX_DATA_SIZE  28

typedef struct _MAIN_ARG_T
{
    int cmd_ch;
    int cmd;
    int type_ch;
    int type;
    int format;
    int item;
    int len;
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

typedef int (*pITEM_TEST_F)(MAIN_ARG_T *ptMainArg);

typedef struct _DATA_ITEM_FUN_T
{
    DATA_TEST_ITEM item;
    pITEM_TEST_F pFun;
} DATA_ITEM_FUN_T;




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
static int FeatureVoiceParaGet(MAIN_ARG_T *ptMainArg);
static int FeatureVoiceStart(MAIN_ARG_T *ptMainArg);
//----


static int getLine(char *prmpt, char *buff, size_t sz);
static int digit_get(char *prmpt, char *buff, size_t sz, int *pDigit)

static int s_tCenterVoiceSockfd;                // 中心语音套接字
static struct sockaddr_in s_tCenterVoiceAddr;



DATA_ITEM_FUN_T atDataItemFun[] = {
    {ITEM_MS_REMOTE_KILL , MS_Remote_Kill_test},        // 0
    {ITEM_MS_ENABLE      , Ms_Enable_test},             // 1
    {ITEM_MS_GPS_REPORT  , Ms_Gps_Report_test},         // 2
    {ITEM_NAS_DISABLE    , Nas_Disable_test},           // 3
    {ITEM_NAS_ENABLE     , Nas_Enable_test},            // 4
    {ITEM_NAS_GPS_REPORT , Nas_Gps_Report_test},        // 5
    {ITEM_NAS_STUN       , Nas_Stun_test},              // 6
    {ITEM_NEIGHBOR_QUERY , Neighbor_Query_test},        // 7
    {ITEM_SHORT_MESSAGE  , Short_Message_test},         // 8
    {ITEM_MAX            , NULL}                        // 9
};




int MS_Remote_Kill_test(MAIN_ARG_T* ptMainArg)        //0
{ printf("\n[%s:%d]\n\n", __FUNCTION__, __LINE__);}
int Ms_Enable_test(MAIN_ARG_T* ptMainArg)             //1
{  printf("\n[%s:%d]\n\n", __FUNCTION__, __LINE__); }
int Ms_Gps_Report_test(MAIN_ARG_T* ptMainArg)         //2
{  printf("\n[%s:%d]\n\n", __FUNCTION__, __LINE__); }
int Nas_Disable_test(MAIN_ARG_T* ptMainArg)           //3
{  printf("\n[%s:%d]\n\n", __FUNCTION__, __LINE__); }
int Nas_Enable_test(MAIN_ARG_T* ptMainArg)            //4
{  printf("\n[%s:%d]\n\n", __FUNCTION__, __LINE__); }
int Nas_Gps_Report_test(MAIN_ARG_T* ptMainArg)        //5
{  printf("\n[%s:%d]\n\n", __FUNCTION__, __LINE__); }
int Nas_Stun_test(MAIN_ARG_T* ptMainArg)              //6
{  printf("\n[%s:%d]\n\n", __FUNCTION__, __LINE__); }
int Neighbor_Query_test(MAIN_ARG_T* ptMainArg)        //7
{  printf("\n[%s:%d]\n\n", __FUNCTION__, __LINE__); }


#define GET_SUCCEED			0
#define TOO_LONG_ERR        -1
#define NO_INPUT_ERR        -2
#define NOT_DIGIT_ERR       -3
#define CR_ERR              -4
static int digit_get(char *prmpt, char *buff, size_t sz, int *pDigit)
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
		printf("\n\nStr2Digit=%d\n\n", *pDigit);
        return  GET_SUCCEED;
}

static  int Short_Message_test_1(MAIN_ARG_T* ptMainArg)
{
    char buff[10];
    int digit;
    int ret;
    while (1)
    {
        printf("\n\n1) select call type:\n\
        \n\t0 - Private Call\
        \n\t1 - Group Call\n\
        \nPlease press the number to select: ");
        if ((ret = digit_get("Enter type>", buff, 2, &digit)) > 0)
        {
            printf("\n\nget digit=%d\n\n", digit);
            if (digit < 2)
            {
                break;
            }
        }
    }
    return 0;
}



int Short_Message_test(MAIN_ARG_T* ptMainArg)
{
    int ret, ch, type;
    char buff[2];
    char *pbuff = buff;
    int state = 1;
    // 短信参数获取
    // 1-短信长度 1~50*2
    // 2-组ID
Call_Type_Select:

    while (state)
    {
        printf("\n\n1) select call type:\n\
        \n\t1 - Private Call\
        \n\t2 - Group Call\n\
        \nPlease press the number to select: ");
        pbuff = buff;
        ret = getLine("Enter type>", buff, sizeof(buff));
        if (ret == TOO_LONG)
        {
            printf("\ninput number is too long, try again");
        }
        else if (ret == NO_INPUT)
        {
            printf ("\nNo input, try again\n");
        }
        else if (ret == OK)
        {
            while ((ch = *pbuff++) != '\0')
            {
                if (isdigit(ch) == 0)
                {
                    printf("\n\ninput is not digit, try again\n\n");
                    goto Call_Type_Select;
                }
            }

            if (buff[0] == '\0')
            {
                printf("\n\nInput data is CR, try again\n\n");
                goto Call_Type_Select;
            }


            // 判断范围
            if ((type = atoi(buff)) >= 3)
            {
                printf("\nThere is no type=%d, try again\n\n", type);
                goto Call_Type_Select;
            }
//          type
            printf("\nYou Choose type (%d) To Test\n\n", type);
            break;
        }
    }
    // 赋值

    // 调用函数
    printf("\n[%s:%d]\n\n", __FUNCTION__, __LINE__);
}













//-------------------------------------------------------------

int ODP_FeatureVoiceSocket()
{

    return 0;
}

#define     PPT_ON      1
#define     PPT_OFF     0

int ODP_ppt_set(int key)
{
    printf("\n[%s:%d] key=(%d)\n\n", __FUNCTION__, __LINE__, key);
    return 0;
}




int FeatureVoiceStart(MAIN_ARG_T *ptMainArg)
{
    int format, len;
    int cnt = 0;
    format = ptMainArg->format;
    len = ptMainArg->len;

    ODP_FeatureVoiceSocket();

    while (cnt++ < len)
    {
        ODP_ppt_set(PPT_ON);
//        ODP_Voice_Send();
        ODP_ppt_set(PPT_OFF);
    }

    return 0;
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




int sh_getch(void)
{
    int cr;
    struct termios nts, ots;

    if (tcgetattr(0, &ots) < 0) // 得到当前终端(0表示标准输入)的设置
        return EOF;

    nts = ots;
    cfmakeraw(&nts); // 设置终端为Raw原始模式，该模式下所有的输入数据以字节为单位被处理
    if (tcsetattr(0, TCSANOW, &nts) < 0) // 设置上更改之后的设置
        return EOF;

    cr = getchar();
    if (tcsetattr(0, TCSANOW, &ots) < 0) // 设置还原成老的模式
        return EOF;

    return cr;
}




#define DATA_ITEM_BYTE      2       // 字节
#define OK          0
#define NO_INPUT    1
#define TOO_LONG    2

static int getLine(char *prmpt, char *buff, size_t sz)
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







#if 0
#define TOO_LONG            0
#define OK                  1
#define CR                  2
#define NO_INPUT            2



int GetLine(char *prmpt, char *buff, int sz)
{
    int ch, extra, item, index_tail;
    if (prmpt != NULL)
    {
        printf("%s", prmpt);
        fflush(stdout);
    }

    if (fgets(buff, sz, stdin) == NULL)
    {
        printf("[%s:%d]fgets wrong\n", __FUNCTION__, __LINE__);
        return -1;
    }

    // 如果输入数据太长，数组内讲不会有 '\n'
    // 我们应该flush 缓存， 不至于影响下次操作
    index_tail = strlen(buff) - 1;
    printf("@@index_tail=%d\n", index_tail);
    if (buff[index_tail] != '\n')
    {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
        {
            extra = 1;
        }
        return (extra == 1) ? TOO_LONG : OK;
    }
    else
    {
        if (buff[index_tail] == '\0')  // 说明直接按的是回车键
        {
            return CR;
        }
        else
        {
            // 数组最后一个字节为 '\n'， 替换'\n'，通过'\n'来判断输入的正确性
            buff[index_tail] = '\0';
            return OK;
        }
    }
}
#endif


int FeatureDataSingleStart(MAIN_ARG_T *ptMainArg)
{
    int item = ptMainArg->item;
    int index;
    printf("\n[%s:%d] item=(%d)\n", __FUNCTION__, __LINE__, item);
    printf("\n[%s:%d] sizeof(atDataItemFun)/sizeof(MAIN_ARG_T)=(%d)\n", __FUNCTION__, __LINE__, sizeof(atDataItemFun)/sizeof(DATA_ITEM_FUN_T));

//    for (index = 0; index < sizeof(atDataItemFun)/sizeof(DATA_ITEM_FUN_T) - 1; index++)
    for (index = 0; atDataItemFun[index].item != ITEM_MAX; index++)
    {
        printf("\n[%s:%d] atDataItemFun[index].item=(%d)\n", __FUNCTION__, __LINE__, atDataItemFun[index].item);
        if (item == atDataItemFun[index].item)
        {
            (atDataItemFun[index].pFun)(ptMainArg);
            break;
        }
    }
    return 0;
}



int FeatureDataParaGet(MAIN_ARG_T *ptMainArg)
{
    char buff[DATA_ITEM_BYTE];
    char *pbuff = buff;
    int ret;
    int ch;
    int item;

Data_Item_Select:

    while (1)
    {
        printf("\n\nData test items as follows\n\n\
        0 int MS Remote Kill\n\
        1 int Ms Enable\n\
        2 int Ms Gps Report\n\
        3 int Nas Disable\n\
        4 int Nas Enable\n\
        5 int Nas Gps Report\n\
        6 int Nas Stun\n\
        7 int Neighbor Query\n\
        8 int Short Message\n\
        \nPlease press the 0 ~ 8 to select\n");
        ret = getLine("Enter number>", buff, sizeof(buff));

        printf("--@@ret=(%d)\n", ret);
        printf("@@buff=(%s)\n", buff);

        if (ret == TOO_LONG)
        {
            printf("\nThe input number is too long, you can only enter one digit\
            \nplease try again, press any key to continue>");
            //while (((ch = getchar()) != '\n') && (ch != EOF));
            sh_getch();
            goto Data_Item_Select;
            //fflush(stdout);

        }
        else if (ret == OK)
        {
//            printf("\nthe number is OK, press any key to continue>");
            //system("echo Press ENTER to continue; read");
            // sh_getch();
            //fflush(stdout);
        }
        else if (ret == NO_INPUT)
        {
            printf ("\nNo input, please try again, press any key to contin  ue\n");
            sh_getch();
            goto Data_Item_Select;

        }


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
        printf("\nThis data is a digit pbuff=(%s)\n\n", pbuff );



        // 判断范围
        if ((item = atoi(buff)) >= ITEM_MAX)
        {
            printf("\nThere is no item=%d, try again, press any key to continue\n\n", item);
            goto Data_Item_Select;
        }


        // 1int得到一个字节， 具体是什么不知道
        // 判断是否为数字
        //
//      printf("\n[%s:%d] item = %d\n\n", __FUNCTION__,__LINE__, item);
        printf("\nYou Choose Item (%d) To Test\n\n",  item);
        ptMainArg->item = item;
        return item;    // 返回item;
    }
}



int FeatureVoiceParaGet(MAIN_ARG_T *ptMainArg)
{
    int format;
    int ch, len;
    int i = 0;
    char buff[POCKET_BYTE_LEN];

    while (1)
    {
        printf("\n1) The voice format as follow\n\
        1 int ambe\n\
        2 int nvoc\n\
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
                while((ch=getchar())!='\n' &&ch != EOF);    // 清除 stdin 缓存, 具体是清除 '\n'
                printf("\nYou choose:  nvoc\n");
                ptMainArg->format = 2;
                goto Voice_Len_Select;
            }
            else
            {
                exit(0);
            }
        }
        printf("sorry, the number error, please try again(1 or 2)\n");
    }

Voice_Len_Select:

    while (1)
    {
        memset(buff, 0, sizeof(buff));
        printf("\n2) Input the packet len (range: 100~500)\n\
               \nthe number is: ");

        if (fgets(buff, sizeof(buff), stdin) == NULL)
        {
            printf("fgets wrong, please try again\n");
            return -1;
        }

//        printf("\n@@[%s:%d] buff=(%s) strlen(buff)=(%d)\n", __FUNCTION__, __LINE__, buff,  strlen(buff) );


        if (buff[strlen(buff)-1] != '\n')
        {
            // 输入数据太长，以至于无'\n'
            // 所以，需要清除 stdin 缓冲
//            printf("\n[%s:%d] buff=(%s) strlen(buff)=(%d)\n", __FUNCTION__, __LINE__, buff, strlen(buff) );
            while (((ch = getchar()) != '\n') && (ch != EOF));  // 清除 stdin中的 '\n'
//            printf("\n[%s:%d] buff=(%s) strlen(buff)=(%d)\n", __FUNCTION__, __LINE__, buff, strlen(buff) );
        }
        else
        {
//            printf("\n[%s:%d] buff=(%s) strlen(buff)=(%d)\n", __FUNCTION__, __LINE__, buff, strlen(buff) );
            // 清除'\n' 替换为 '\0'
            buff[strlen(buff)-1] = '\0';
        }

        while ((ch = buff[i++]) != '\0')
        {
//            printf("[%s:%d] ch=%c\n", __FUNCTION__, __LINE__, ch);
            if (isdigit(ch) == 0)
            {
                // 非数字
                //printf("[%s:%d] you input\n", __FUNCTION__, __LINE__);
                printf("input data is not digit, please try again \n");
//                break;
                goto Voice_Len_Select;
            }

        }

//        printf("[%s:%d] buff=%s\n", __FUNCTION__, __LINE__, buff);

        sscanf(buff, "%d", &len);
//        printf("\nsscanf len=%d\n", len);
//      len = atoi(buff);
//      printf("atoi len=%d\n", len);

        if (len >= POCKET_BYTE_MIN && len <= POCKET_BYTE_MAX)
        {
            ptMainArg->len = POCKET_BYTE_MAX;
            printf("\nThe packet len is set ok\n\n");
            break;
        }
        else if (len > POCKET_BYTE_MAX )
        {
            printf("\nThe packet len you input is too large,\
            \nSo, the packet len is set to the maximum (%d) for the time being\n\n", POCKET_BYTE_MIN);
            ptMainArg->len = POCKET_BYTE_MAX;
            break;
        }
        else if (len < POCKET_BYTE_MIN)
        {
            printf("\nThe packet len you input is too little,\
            \nSo, the packet len is set to the minimum (%d) for the time being\n\n", POCKET_BYTE_MIN);
            ptMainArg->len = POCKET_BYTE_MIN;
            break;
        }
        else
        {
            printf("[%s:%d] len=(%d)(%x) err \n", __FUNCTION__, __LINE__, len, len);
            goto Voice_Len_Select;
        }
    }

    printf("~~~~len set over~~~~~\n", __FUNCTION__, __LINE__);
    return 0;
}




#define  AMBE_FORMAT        1
#define  NVOC_FORMAT        2

#define  AMBE_FILE_NAME     "AMBE_Test.ap"
#define  NVOC_FILE_NAME     "NVOC_Test.ap"







// -1 表示 失败
//
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


typedef struct _CACHE_T
{
    char *pBuff;
    int len;
} CACHE_T;






void ODP_Voice_Send(int len, int format)
{
    FILE *fp;
    int ret;
    char *pVoiceFileName = NULL;

    CENTER_VOICE_DATA tCenterVoiceData;
    CENTER_VOICE_DATA *pSendBuff = &tCenterVoiceData;

    int SendByteCnt  = 0;
    int SendFrameCnt = 0;
    int SendLen = len;  // 20~1000
    int file_size;
    int readlen;
    int index = 0, i=0;
    CACHE_T tCache;
    CACHE_T *pCache = &tCache;


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
    else
    {
        printf("(%s) ret (%d)\n", pVoiceFileName, ret );
    }

    // 打开文件设置属性
    if ((fp = fopen(pVoiceFileName, "rb")) == NULL)
    {
        printf("CC open file err\n");
        exit(0);
    }


    pCache->len = get_file_size(pVoiceFileName);
    printf("pCache->len=%d\n", pCache->len);

    if ((pCache->pBuff = (char *)malloc(pCache->len)) == NULL)
    {
        perror("malloc");
        exit(0);
    }

    // 将文件读取到内存中

    printf("sizeof(pCache->pBuff[0])=(%d)\n", sizeof(pCache->pBuff[0]));
    printf("pCache->len=(%d)\n", pCache->len);

    ret = fread(pCache->pBuff, sizeof(pCache->pBuff[0]), pCache->len, fp);
    printf("fread: ret=%d\n", ret);
    if (ret != pCache->len)
    {
        perror("fread");
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
        if (index*27 < pCache->len)
        {
            if (index == SendLen )
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







int main()
{
    int ch;
    int index=0;
    MAIN_ARG_T tMainArg;
    MAIN_ARG_T *ptMainArg = &tMainArg;
//    FeatureVoiceParaGet(ptMainArg);
// FeatureVoiceStart(ptMainArg);
//    ODP_Voice_Send(5, 1);
//    ODP_Voice_Send(10, 20);
    Short_Message_test(ptMainArg);

#if 0
    while (1)
    {

//        FeatureDataParaGet(ptMainArg);
//        FeatureDataSingleStart(ptMainArg);
        sh_getch();
    }
#endif


#if 0
    while (1)
    {
        printf("input: ");
        ch = getc(stdin);
        printf("ch=(%c)(%#x)\n\n", ch, ch);
        while((ch=getchar())!='\n' &&ch != EOF);

    }
#endif

    return 0;
}

