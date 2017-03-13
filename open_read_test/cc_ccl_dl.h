

#ifndef __CC_CCL_DL_H__
#define __CC_CCL_DL_H__
/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    cc_ccl_dl.h
 * @brief   呼叫控制层下行函数接口
 * @author  牛功喜
 * @version v1.0
 * @date    2016-07-29
 */
/*
 *   函数列表
 *   1. timer_handler             定时器处理函数
 *   2. timercallback              定时器回调函数
 *   3.
 *   4.
 *   5.
 *   6.
 *   7.
 *
 *   修改历史
 *   2016-08-08          建立文件
 *   *************************************************************************/

/******************************************************************************
 *   引用头文件声明
 *   *************************************************************************/
//#include "Unit_inf.h"


#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <pthread.h>
#include <linux/unistd.h>
#include <linux/kernel.h>

/******************************************************************************
 *   宏定义
 *   *************************************************************************/

 /**
 * @def   INF_SHARE_HEAD_LEN
 * @brief 接入单元与中心交互公用头长度
 */
#define    CENTER_SHARE_HEAD_LEN    18
/**
 * @def  PTT_ACK_VALID_LEN
 * @brief  CCL层回复PTT命令有效数据长度
 */
#define    PTT_ACK_VALID_LEN        100

/**
 * @def  RTP_MAX_DATA_SIZE
 * @brief  中心语音负载长度
 */
#define   RTP_MAX_DATA_SIZE  28
/******************************************************************************
 *   类型定义
 *   *************************************************************************/
  /******************************************************************************
 *   结构体声明
 *   *************************************************************************/


/**
 * @struct _CENTER_CMD_SHAREHEADER
 * @brief  接入单元与中心命令交互共用头
 */
typedef struct _CENTER_CMD_SHARE_HEAD
{
    unsigned short SigHead;        // 标识AC接口信令头标志(0x13ec)
    unsigned short SigType;        // 信令类型 标识AC接口PTT命令信令（0x000a）
    unsigned short Datalength;     // 有效数据长度 标识信令有效数据长度（100）
    unsigned short SourceID;       // 源单元号 标识AC接口发送单元ID值
    unsigned short DestID;         // 目的单元号  标识AC接口接收单元ID值
    unsigned char  SourceType;     // 源单元设备类型 标识发送单元设备类型（制式）
    unsigned char  SigVersion;     // 信令版本号 标识该信令适合版本号0x02(该信令与0x01版本不兼容)
    unsigned char  PathNum ;       // 通道号码 标识该信令属于该单元的通道号码，若不区分通道，则填全F
    unsigned char  Exhead[11];     // 扩展头 扩展头中心无须关心参数
    unsigned int   CheckVal;       // 校验值 标识信令校验结果(01版本 与02版本 目前都填写全F，标示无校验方式)
} CENTER_CMD_SHARE_HEAD ;



/**
 * @struct  _PTT_CMD
 * @brief  PTT-ON  PTT-OFF  命令结构体
 */
typedef struct _PTT_CMD
{
    CENTER_CMD_SHARE_HEAD  SharedHead;
    unsigned int  CallId;            //控制面ID 标识一次呼叫的控制面ID
    unsigned int  VoiceId;           //业务面ID 标识一次呼叫的业务面ID
    unsigned char PttStat;           //PTT信令状态 标识PTT信令状态（请求CMD_ON=0x06、释放CMD_OFF=0x07）
    unsigned char Reserved;          //字节对齐占位中心无须关心参数
    unsigned char TalkingNum[30];    //标识当次通话PTT的讲话方号码
    unsigned char CallingNum[30];    //标识当次呼叫的主叫号码
    unsigned char CalledNum[30];     //标识当次呼叫的被叫号码（组号、个号）
} PTT_CMD;



/**
 * @struct   _CENTER_VOICE_DATA_DL
 * @brief 中心语音帧下行结构
 */
typedef  struct _CENTER_VOICE_DATA_DL

    unsigned char  Version:2;
    unsigned char  Padding:1;      //填充标记
    unsigned char  ExtendHead:1;   //扩展头标记
    unsigned char  CsrcCount:4;   //贡献源计数
    unsigned char  Marker:1;       //RTP重要事件标记
    unsigned char  PayloadType:7;  //RTP 负载类型
    unsigned short SequenceNum ;     //RTP 序列号
    unsigned int   TimeStamp;        //RTP时间戳
    unsigned int   Ssrc;             //RTP 同步源
    unsigned short ProfileHead;      //rtp扩展头特性参数
    unsigned short ExheadLen;        //RTP扩展头数据长度
    unsigned int   VoiceId;          //标识一次呼叫的业务面ID
    unsigned int   Gps;              //GPS时间
    unsigned char  CodecFlg;         //数据编解码编解码标识
    unsigned char  FrameFlg;         //数据帧标识
    unsigned char  PathNum;          //语音通道号码，若不区分通道，则填全F
    unsigned char  PackRate;         //标识语音包速率
    unsigned char  EncryptFlag;      //标识是否加密
    unsigned char  Reserve[3];
    unsigned char  VoiceData[507];
    unsigned char  ReserveData[5];
 } CENTER_VOICE_DATA_DL;



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
 * @struct   _Rtp_Repeater_Ex_Head
 * @brief RTP 转发扩展
 */
typedef struct  _Rtp_Repeater_Ex_Head
{
    unsigned short   profile_head;
    unsigned short   exhead_length;      /* 32bit number == 4 bytes number*/
    unsigned int     voice_id;
    unsigned int     gps;
    unsigned char    codec_flag;
    unsigned char    frame_flag;
    unsigned short   reserve;
} Rtp_Repeater_Ex_Head;


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

/******************************************************************************
 *   全局变量定义
 *   *************************************************************************/

/******************************************************************************
 *   局部变量定义
 *   *************************************************************************/

/******************************************************************************
 *   内部函数实现
 *   *************************************************************************/
 /**
 * @brief  定时器处理函数
 */
void timer_handler (int signo);

/**
* @brief     定时器回调函数
*/
void timercallback();

/******************************************************************************
 *   可调用函数实现
 *   *************************************************************************/

#endif //CC_CCLDL_H
























