 
/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    cclul.c
 * @brief   呼叫控制层上行函数接口
 * @author  牛功喜
 * @version v1.0
 * @date    2016-08-08
 */
 
/*
 *   函数列表
 *   1. 
 *   2. 
 *   3. 
 *   4. 
 *   5.
 *   6. 
 *   7. 
 *
 *   修改历史
 *   2016-07-29            建立文件                   
 *   *************************************************************************/

/******************************************************************************
 *   引用头文件声明
 *   *************************************************************************/
#include "cc_ccl_dl.h"
//#include "DLL.h"

/******************************************************************************
 *   宏类型定义
 *   *************************************************************************/
 #define    TIMER_INTERVAL                             30
 
 
 
 /**
 * @def  RECV_DLL_PORT
 * @brief 本地发送DLL 端口
 */
#define CC_SEND_VOICE_PORT                           (10011)// (6017)
/**
 * @def  SEND_CCL_PORT
 * @brief  本地接收DLL端口
 */
#define  CC_SEND_SIG_PORT                            (10013) //(6018)



 /******************************************************************************
 *   全局变量定义
 *   *************************************************************************/
/******************************************************************************
 *   局部变量定义
 *   *************************************************************************/
unsigned char pttonflg;
/**
* @var voicedatasentover
* @brief 语音发送结束标志
*/
unsigned char voicedatasentover;
/**
* @var s_tCenterVoiceSockfd
* @brief 中心语音套接字
*/
static int  s_tCenterVoiceSockfd ; 
/**
* @var  s_tCenterSigSockfd
* @brief 中心信令套接字
*/
static int s_tCenterSigSockfd  ;    
/**
* @struct  s_tCenterSigAddr
* @brief 中心信令下行地址
*/
static  struct sockaddr_in  s_tCenterSigAddr;  
/**
* @struct  s_tCenterVoiceAddr
* @brief 中心语音下行地址
*/
static  struct sockaddr_in	s_tCenterVoiceAddr;  
/**
* @struct  timecount
* @brief 中心时间计数
*/
static  unsigned char 	timecount;  

/**
 * @var  s_tHandleCenterViocethread
 * @brief 处理中心语音线程标识
 */
//static  pthread_t        s_tCenterViocethread;

/**
 * @var  s_tHandleCenterSigthread
 * @brief 处理中心信令线程标识
 */
static  pthread_t         s_tCenterSigthread;
/**
 * @var  s_ttimerthread
 * @brief 定时器线程
 */
static  pthread_t         s_ttimerthread;

/**
 * @var  packetnum
 * @brief 发送语音包数目
 */
static unsigned short       packetnum;
/**
 * @var  DataFlag
 * @brief 发送数据包类型标识
 */

static unsigned  char      DataFlag;
static unsigned  char      PrintFlag;

///< 测试源类型
#define TEST_DATA_1031          (( unsigned  short)0)
#define TEST_DATA_SILENCE   	(( unsigned  short)1)
#define TEST_DATA_153           (( unsigned  short)2)
#define TEST_DATA_4FSK          (( unsigned  short)3)

// @@@这个是什么意思
static unsigned  short  s_au2AmbeEraData[14] =
{
	0xb9e8, 0x8152, 0x6173, 0x002a, 0x6bb9, 0xe881, 0x5261,
	0x7300, 0x2a6b, 0xb9e8, 0x8152, 0x6173, 0x002a, 0x6b00
};


static unsigned short  s_au2VoiceSource[3][6][14] =
{
    {
        // 1031信源测试信源
        {   // 1031信源VoiceA
            0xcea8, 0xfe83, 0xacc4, 0x5820, 0x0ace, 0xa8fe, 0x83ac, 0xc458, 0x200a, 0xcea8, 0xfe83,
            0xacc4, 0x5820, 0x0a00
        },
        {   // 1031信源VoiceB
            0xcea8, 0xfe83, 0xacc4, 0x5820, 0x0ace, 0xa8fe, 0x83ac, 0xc458, 0x200a, 0xcea8, 0xfe83,
            0xacc4, 0x5820, 0x0a00
        },
        {   // 1031信源VoiceC
            0xcea8, 0xfe83, 0xacc4, 0x5820, 0x0ace, 0xa8fe, 0x83ac, 0xc458, 0x200a, 0xcea8, 0xfe83,
            0xacc4, 0x5820, 0x0a00
        },
        {   // 1031信源VoiceD
            0xcea8, 0xfe83, 0xacc4, 0x5820, 0x0ace, 0xa8fe, 0x83ac, 0xc458, 0x200a, 0xcea8, 0xfe83,
            0xacc4, 0x5820, 0x0a00
        },
        {   // 1031信源VoiceE
            0xcea8, 0xfe83, 0xacc4, 0x5820, 0x0ace, 0xa8fe, 0x83ac, 0xc458, 0x200a, 0xcea8, 0xfe83,
            0xacc4, 0x5820, 0x0a00
        },
        {   // 1031信源VoiceF
            0xcea8, 0xfe83, 0xacc4, 0x5820, 0x0ace, 0xa8fe, 0x83ac, 0xc458, 0x200a, 0xcea8, 0xfe83,
            0xacc4, 0x5820, 0x0a00
        }
    },
    {
        // Silence信源测试信源
        {   // Silence信源VoiceA
            0xb9e8, 0x8152, 0x6173, 0x002a, 0x6bb9, 0xe881, 0x5261, 0x7300, 0x2a6b, 0xb9e8, 0x8152,
            0x6173, 0x002a, 0x6b00
        },
        {   // Silence信源VoiceB
            0xb9e8, 0x8152, 0x6173, 0x002a, 0x6bb9, 0xe881, 0x5261, 0x7300, 0x2a6b, 0xb9e8, 0x8152,
            0x6173, 0x002a, 0x6b00
        },
        {   // Silence信源VoiceC
            0xb9e8, 0x8152, 0x6173, 0x002a, 0x6bb9, 0xe881, 0x5261, 0x7300, 0x2a6b, 0xb9e8, 0x8152,
            0x6173, 0x002a, 0x6b00
        },
        {   // Silence信源VoiceD
            0xb9e8, 0x8152, 0x6173, 0x002a, 0x6bb9, 0xe881, 0x5261, 0x7300, 0x2a6b, 0xb9e8, 0x8152,
            0x6173, 0x002a, 0x6b00
        },
        {   // Silence信源VoiceE
            0xb9e8, 0x8152, 0x6173, 0x002a, 0x6bb9, 0xe881, 0x5261, 0x7300, 0x2a6b, 0xb9e8, 0x8152,
            0x6173, 0x002a, 0x6b00
        },
        {   // Silence信源VoiceF
            0xb9e8, 0x8152, 0x6173, 0x002a, 0x6bb9, 0xe881, 0x5261, 0x7300, 0x2a6b, 0xb9e8, 0x8152,
            0x6173, 0x002a, 0x6b00
        }
    },
    {
        // 153信源测试信源
        {   // 153信源VoiceA
            0xFF83, 0xDF17, 0x3209, 0x4ED1, 0xE7CD, 0x8A91, 0xC6D5, 0xC4C4, 0x4021, 0x184E, 0x5586,
            0xF4DC, 0x8A15, 0xA700
        },
        {   // 153信源VoiceB
            //0xE322, 0x2193, 0x5348, 0x121A, 0x34BF, 0xA2C7, 0x5967, 0x8FBA, 0x0D6D, 0xD82D, 0x7D54,
            //0x0A57, 0x9310, 0x3900
                 // source
	           0xEC92, 0xDF93, 0x5330, 0x18CA, 0x34BF, 0xA2C7, 0x5967, 0x8FBA, 0x0D6D, 0xD82D, 0x7D54,
	           0x0A57, 0x9770, 0x3900
			
            
        },
        {   // 153信源VoiceC
            0xD27A, 0xEA24, 0x3385, 0xED9A, 0x1DE1, 0xFF07, 0xBE2E, 0x6412, 0x9DA3, 0xCF9B, 0x1523,
            0x8DAB, 0x8988, 0x8000
        },
        {    // 153信源VoiceD
            0x4230, 0x9CAB, 0x0DE9, 0xB914, 0x2B4F, 0xD925, 0xBF26, 0xA660, 0x3194, 0x697F, 0x458E,
            0xB2CF, 0x1F74, 0x1A00
        },
        {    // 153信源VoiceE
            0xDBB0, 0x5AFA, 0xA814, 0xAF2E, 0xE073, 0xA4F5, 0xD448, 0x670B, 0xDB34, 0x3BC3, 0xFE0F,
            0x7C5C, 0xC825, 0x3B00
        },
        {    // 153信源VoiceF
            0x479f, 0x362a, 0x471b, 0x5713, 0x1100, 0x8461, 0x3956, 0x1BD3, 0x7228, 0x569F, 0xB24B,
            0x7E4D, 0x4CC0, 0x6300
        }
    }
};


/******************************************************************************
 *   内部函数实现
 *   *************************************************************************/
 
/**
* @brief	 定时器按照设定时间发出信号
* @param [in] n_msecs   定时时间
* @author 牛功喜
* @since	 
* @bug
*/

int  set_ticker(int n_msecs)										 //定时器 
{  
  struct  itimerval new_timeset;  
  long n_sec, n_usecs;	
  n_sec = n_msecs / 1000;  
  n_usecs = (n_msecs % 1000) * 1000L;  
  new_timeset.it_interval.tv_sec = n_sec;  // set reload   设置初始间隔  
  new_timeset.it_interval.tv_usec = n_usecs; //new ticker value  
  new_timeset.it_value.tv_sec = n_sec;	 //store this	设置重复间隔  
  new_timeset.it_value.tv_usec = n_usecs; //and this  
  return setitimer(ITIMER_REAL, &new_timeset, NULL);   
}    

/**
* @brief	 定时器处理函数
* @author 牛功喜
* @since	 
* @bug
*/
void timercallback()
{        int i;
	timecount++;
	
	//printf(" timecount  xxxxxxx=%d  \n",timecount);
	i=(timecount%30);
	if( (timecount%30) ==0)
	printf(" time is 1s  timecount =%d    i=%d\n",timecount,i);

}
/**
* @brief MS定时器
* @author 牛功喜
* @since	 
* @bug
*/

void msec_sleep(unsigned long msec) {
    int err;
    struct timeval tv;
    tv.tv_sec = msec / 1000;
    tv.tv_usec = (msec % 1000) * 1000;
    do {
           err = select(0, NULL, NULL, NULL, &tv);
    } while (err < 0 && errno == EINTR);
}


/**
* @brief  定时器处理函数
* @param [in] signo 信号值
* @author 牛功喜
* @since   
* @bug
*/
void timer_handler (int signo)
{
  switch(signo) 
  {
  case SIGALRM:
	  timercallback();
	  break;
default:
	  break;
  }
}


int    time_substract(struct timeval *result, struct timeval *begin,struct timeval *end)

{

    if(begin->tv_sec > end->tv_sec)    return -1;

    if((begin->tv_sec == end->tv_sec) && (begin->tv_usec > end->tv_usec))    return -2;

     result->tv_sec    = (end->tv_sec - begin->tv_sec);

    result->tv_usec    = (end->tv_usec - begin->tv_usec);

    if(result->tv_usec < 0)

    {

        result->tv_sec--;

        result->tv_usec += 1000000;

    }
    return 0;

}

/*void delay()
{   
       struct timeval tv;    
        tv.tv_sec = 0;  // 定时1秒  
        tv.tv_usec =60000;    
          
        switch(select(0, NULL, NULL, NULL, &tv))   
        {  
        case -1:  // 错误  
            printf("d Error!\n");  
            break;  
        case 0: //超时  
          //  printf("timeout expires.\n");  
            break;  
        default:  
            printf("  d   default\n");  
            break;  
        }  
}*/

/**
* @brief  阻塞延时函数
* @param [in] msec	 ms 延时时间
* @author 牛功喜
* @since   
* @bug
*/
void delay(unsigned long msec)
{		 
	 struct timeval tv;
	 tv.tv_sec = msec / 1000;
	 tv.tv_usec = (msec % 1000) * 1000;
	 switch(select(0, NULL, NULL, NULL, &tv))	
	 {	
	 case -1:  // 错误	
		 printf("d Error!\n");	
		 break;  
	 case 0: //超时  
	   //  printf("timeout expires.\n");  
		 break;  
	 default:  
		 printf("  d   default\n");  
		 break;  
	 }	
}

void printf_localtime(char *str)
{
    time_t timep; 
    struct tm *localtime; 
    struct timeval tv; 
    struct timezone tz; 
    
    time(&timep); 
    localtime = gmtime(&timep);     //返回结构tm代表目前UTC 时间    
    gettimeofday (&tv , &tz); 
    
    printf("%d/%d/%d ",(1900+localtime->tm_year), (1+localtime->tm_mon),localtime->tm_mday); 
    printf("%02d:%02d:%02d.%06ld  %s\n", localtime->tm_hour, localtime->tm_min, localtime->tm_sec, tv.tv_usec, str); 

}


/**
* @brief  定时函数
* @param 
* @author 牛功喜
* @since   
* @bug
*/

void timer()
{   
       struct timeval tv;    
        tv.tv_sec = 0;  // 定时1秒  
        tv.tv_usec =30000;    
          
        switch(select(0, NULL, NULL, NULL, &tv))   
        {  
        case -1:  // 错误  
            printf("Error!\n");  
            break;  
        case 0: //超时  
          //  printf("timeout expires.\n");  
            break;  
        default:  
            printf("default\n");  
            break;  
        }  
}






void *timerprcoess(void *arg)
{
	struct timeval  start, stop, diff;
	struct timezone tz;

	unsigned char  usentbuf[1024];
	unsigned char  u2SrcType = TEST_DATA_153;  // @@@什么意思 ?
	unsigned char  readlen;
	unsigned short u2senddatalen;
	unsigned short sentframtime = 0;
	unsigned short au2FrmType = 0;
	int sentvoicelen = 0;
	FILE *fp;
	
	CENTER_VOICE_DATA * ptsent = (CENTER_VOICE_DATA *)usentbuf;
	memcpy(ptsent->Payload, (unsigned char *)s_au2AmbeEraData, 27);		// 
	
	fp = fopen("AMBE_Test.ap","r");		// @@放入的是什么文件
	if (fp == NULL)
	{
		printf("[CENTER] CC testsorce open file err\n");
		exit(0);
	}

	while (1)
  	{ 		   
		timecount++;
		if ((1 == pttonflg)) 
		{   
			//printf("[CENTER] voice\n");
			if (voicedatasentover != 1)
		   	{
				//发送语音
				sentframtime = sentframtime + 1;
				if (DataFlag == 1)
				{
					u2senddatalen = sendto( s_tCenterVoiceSockfd, usentbuf, sizeof(CENTER_VOICE_DATA), 0, (struct sockaddr *)&s_tCenterVoiceAddr, sizeof(s_tCenterVoiceAddr)); 
					delay(35);
				}
				else if (DataFlag == 2)
				{
					gettimeofday(&start,&tz);
					readlen = fread(ptsent->Payload, 1, 27, fp);

					if (readlen != 27)
					{
						printf("[CENTER]  read file err  len=%d\n",readlen);
						fclose(fp);	
						exit(0) ;
					}
					sentvoicelen = sentvoicelen + 27;
					u2senddatalen = sendto( s_tCenterVoiceSockfd, usentbuf, sizeof(CENTER_VOICE_DATA), 0,(struct sockaddr *)&s_tCenterVoiceAddr,sizeof(s_tCenterVoiceAddr)); 
					
					delay(35);
					gettimeofday(&stop,&tz);

					//time_substract(&diff,&start,&stop);
					printf(" packet Total time : %d s,%d us\n",(int)stop.tv_sec,stop.tv_usec);
					//printf("Total time : %d s,%d us\n",(int)diff.tv_sec,(int)diff.tv_usec);

					if (PrintFlag)
					{
						printf("[CENTER] voice sentlen=%d  framecount=%d,filelen=%d\n",u2senddatalen,sentframtime,sentvoicelen);
					}
					if (sentframtime>= packetnum)  
					{		 
						printf("[CENTER ]voice sentleover timecount=%d----  \n",timecount);
						voicedatasentover=1;
					} 
				}
				else if(DataFlag ==3 )
				{
					if (6 == au2FrmType)
					{
						au2FrmType=0;
					}
					memcpy(ptsent->Payload, s_au2VoiceSource[u2SrcType][au2FrmType], 14);
					au2FrmType++;
					u2senddatalen = sendto( s_tCenterVoiceSockfd, usentbuf, 529, 0,(struct sockaddr *)&s_tCenterVoiceAddr,sizeof(s_tCenterVoiceAddr)); 
					delay(35);
			
					if(PrintFlag)
					{
						printf("[CENTER] voice sentlen=%d  framecount=%d    frmtype=%d \n",u2senddatalen,sentframtime,(au2FrmType-1));
					}
				}
				if(sentframtime>=packetnum)  
				{	   
					printf("[CENTER] voice sentleover   timecount=%d	\n",timecount);
					voicedatasentover=1;
					sentframtime=0;
				} 
			}
    	}
	}
	return 0 ;
}






void *ODP_CenterSig(void *arg)
{
	unsigned char usentbuf[512];
	PTT_CMD * ptsentbuf = (PTT_CMD *)usentbuf;
	unsigned  short u2datasendlen;
    struct  timeval    sigstart,sigstop;
    struct  timezone   tz;
	pttonflg=0;
	
	while(1)
	{       
		  //if(timecount>1)
		  	{       
		  		if(0==pttonflg)
		  		{	         
					ptsentbuf->SharedHead.SigHead=0xec13;
					ptsentbuf->PttStat=0x06;    //PTT mingling 0X06
					ptsentbuf->SharedHead.SigType=0x000a;//PTT_ON
					ptsentbuf->CallingNum[0]=0;
					ptsentbuf->CallingNum[1]=0;
					ptsentbuf->CallingNum[2]=143;
					ptsentbuf->CalledNum[0]=0;
					ptsentbuf->CalledNum[1]=0x03;
					ptsentbuf->CalledNum[2]=0xe8;
					u2datasendlen = sendto( s_tCenterSigSockfd, ptsentbuf,128,0,(struct sockaddr *)&s_tCenterSigAddr,sizeof(s_tCenterSigAddr));
					pttonflg=1;
					timecount=0;
					voicedatasentover=0;
					//if(PrintFlag)
					{
						printf("PTT on sendlen =%d\n",u2datasendlen);
						gettimeofday(&sigstart,&tz);
						printf("ptt on  time : %d s,%d us\n",(int)sigstart.tv_sec,sigstart.tv_usec);
					}
					//sleep(1);
	  	         }
		  	  if(1==voicedatasentover)
		  	  {	                    

					ptsentbuf->SharedHead.SigHead=0xec13;
					ptsentbuf->PttStat=0x07;	//PTT mingling 
					ptsentbuf->SharedHead.SigType=0x000a;//PTT_ON
					u2datasendlen = sendto( s_tCenterSigSockfd, ptsentbuf, 128,0,(struct sockaddr *)&s_tCenterSigAddr,sizeof(s_tCenterSigAddr));
					printf("PTT OFF sendlen =%d\n",u2datasendlen);
					pttonflg=0;
					timecount=0;
					
					gettimeofday(&sigstop,&tz);
					printf("ptt on  time : %d s,%d us\n",(int)sigstop.tv_sec,sigstop.tv_usec);

					if(PrintFlag)
					{
					}
					// voicedatasentover=0;//退出线程
					exit(0);
					//   sleep(10);
		  	   }
		  	}
		 //else
		  	{
		  		   usleep(10);
	  		}
	}
}

void *ODP_CenterVioce(void *arg)

{
	unsigned char usentbuf[1024];
	//unsigned char urecbuf[512];
	//unsigned short u2recdatalen;
	unsigned short u2senddatalen;
	unsigned short  sentframtime=0;
	CENTER_VOICE_DATA_DL * ptsent = (CENTER_VOICE_DATA_DL*)usentbuf;
	//FILE *fp;
	//unsigned  short u2datareclen;
	memcpy(ptsent->VoiceData,(unsigned  char*)s_au2AmbeEraData,27);
	//unsigned char readlen;
	int k;
  while(1)
  {
		if((timecount>2) &&  (1==pttonflg))
			{  
				k=(timecount%2);
				printf("kwwww=%d   timecount =%d\n ",k,timecount);
				  //sleep(10);
				if ((timecount%2)==0)
				{      
					 	// sleep(1);
						// 发送语音
						sentframtime++;
						u2senddatalen = sendto( s_tCenterVoiceSockfd, usentbuf, 529, 0,(struct sockaddr *)&s_tCenterVoiceAddr,sizeof(s_tCenterVoiceAddr)); 
						printf("voice sentlen=%d  framecount=%d\n",u2senddatalen,sentframtime);
						if(sentframtime>= packetnum)  
							{		
								printf("voice sentleover   timecount=%d  \n",timecount);
								sleep(1);
								voicedatasentover=1;
								sentframtime=0;
							} 
						
				  }
				
		 }
	/*	 if( 1==pttonflg)
		 {
                           if(fp=fopen("centersorce","r")) 
                           {
	                              readlen=fread(ptsent->VoiceData,1,27,fp);
	                              if(readlen!=27)
	                               {
				        printf("CC	 read file err  len=%d\n",readlen);
				        break ;
	                               }
	                               delay(60);
				   sentframtime++;
				   u2senddatalen = sendto( s_tCenterVoiceSockfd, usentbuf, 529, 0,(struct sockaddr *)&s_tCenterVoiceAddr,sizeof(s_tCenterVoiceAddr)); 
				   printf("voice sentlen=%d  framecount=%d\n",u2senddatalen,sentframtime);
				   if(sentframtime>= packetnum)  
					   {	   
						   printf("voice sentleover   timecount=%d	\n",timecount);
						   sleep(1);
						   voicedatasentover=1;
						   sentframtime=0;
					   } 
                               
                           }
                           else
                           {
                                      printf("CC    testsorce open file err\n");
                           }

		 }*/
		   else
		   	{
		   		;
		   		
		   	}
	 }
	
}


int center_init(void)
{
	  unsigned char res;      
	// set_ticker(TIMER_INTERVAL);									//设置定时时间
	//signal(SIGALRM, timer_handler);	//定时回调函数
	
	s_tCenterVoiceSockfd = socket(AF_INET, SOCK_DGRAM, 0);		 //中心语音套接字
	if( s_tCenterVoiceSockfd <0) 
	{
		  printf("creat  centervoicefd err	 \n");
		  exit(0);
	}
	bzero(&s_tCenterVoiceAddr, sizeof(s_tCenterVoiceAddr));
	s_tCenterVoiceAddr.sin_family = AF_INET;
	s_tCenterVoiceAddr.sin_addr.s_addr =htonl(INADDR_ANY);//       htonl(INADDR_ANY);  //inet_addr("127.0.0.1");//这里不一样
	s_tCenterVoiceAddr.sin_port = htons(CC_SEND_VOICE_PORT);
	//bind(s_tCenterVoiceSockfd,(struct sockaddr *)&s_tCenterVoiceAddr,sizeof(s_tCenterSigAddr)); 
	
	 s_tCenterSigSockfd = socket(AF_INET, SOCK_DGRAM, 0);	 //处理中心语音套接字初始化
	 if( s_tCenterSigSockfd<0) 
	 {
		   printf("creat  centersigfd err   \n");
		   exit(0);
	 }
		 bzero(&s_tCenterSigAddr, sizeof(s_tCenterSigAddr));
		 s_tCenterSigAddr.sin_family = AF_INET;
		 s_tCenterSigAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		 s_tCenterSigAddr.sin_port = htons(CC_SEND_SIG_PORT);
		// bind( s_tCenterSigSockfd,(struct sockaddr *)&s_tCenterSigAddr,sizeof(s_tCenterSigAddr)); 
		   /*创建线程*/
	 /* if((res = pthread_create(&s_tCenterViocethread, NULL,ODP_CenterVioce,NULL) != 0) )    
	  {
		  printf("  Thread CenterVioce   create  failed \n ");
		  return res ;
	  }
	  else
	  {
		  printf("Thread   CenterVioce   create\n");
	   }*/
	  if((res = pthread_create(&s_tCenterSigthread, NULL,ODP_CenterSig,NULL) != 0) )
	  {
		  printf("Thread    CenterSig  create failed \n ");
		  return res ;
	  }
	  else
	  {
		  printf("Thread    CenterSig   create\n");
	  }	
	  
	  if((res = pthread_create(&s_ttimerthread, NULL,timerprcoess,NULL) != 0) )
	  {
		  printf("Thread    ttimerthrea  create failed \n ");
		  return res ;
	  }
	  else
	  {
		  printf("Thread    ttimerthrea  create\n");
	  }		
	//  pthread_join(s_tCenterViocethread,NULL);  
		pthread_join(s_tCenterSigthread,NULL); 
		pthread_join(s_ttimerthread,NULL); 
	   return 0; 
}




int  main(int argc,char *argv[])

{    
     if(argc!=4)
     {
	    printf("agr  error      first  agr:  1-silent frame     2-AMBE_TEST      3-  153 source  \n");
	    printf("agr  error      second agr: packets  to be  sent  \n");
	    printf("agr  error      third  agr: printornot   1--print  0-not print\n");
	    exit(0);

     }
       DataFlag=atoi(argv[1]);
     if(1>DataFlag ||DataFlag>3)
     {
		 printf(" first agr  error 	 agr:  1-silent frame	  2-AMBE_TEST	   3-  153 source  \n");
		  exit(0);
     }
     
      packetnum=atoi(argv[2]);
      if(1000<packetnum  || packetnum <1)
      {
                    printf("second  agr  error	more than  1000    second agr : packets	to be  sent  \n");
		  exit(0);
      }
      PrintFlag=atoi(argv[3]);
      if(0!=PrintFlag  && 1!=PrintFlag)
      {
                    printf(" third agr  error	 printornot   1--print  0-not print  \n");
		  exit(0);
      }
   center_init();
   
}
