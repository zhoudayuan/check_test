

#ifndef __CC_CCL_DL_H__
#define __CC_CCL_DL_H__
/**
 * Copyright (C), 2016-2020, Hytera Comm. Co., Ltd.
 * @file    cc_ccl_dl.h
 * @brief   ���п��Ʋ����к����ӿ�
 * @author  ţ��ϲ
 * @version v1.0
 * @date    2016-07-29
 */
/*
 *   �����б�
 *   1. timer_handler             ��ʱ��������
 *   2. timercallback              ��ʱ���ص�����
 *   3.
 *   4.
 *   5.
 *   6.
 *   7.
 *
 *   �޸���ʷ
 *   2016-08-08          �����ļ�
 *   *************************************************************************/

/******************************************************************************
 *   ����ͷ�ļ�����
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
 *   �궨��
 *   *************************************************************************/

 /**
 * @def   INF_SHARE_HEAD_LEN
 * @brief ���뵥Ԫ�����Ľ�������ͷ����
 */
#define    CENTER_SHARE_HEAD_LEN    18
/**
 * @def  PTT_ACK_VALID_LEN
 * @brief  CCL��ظ�PTT������Ч���ݳ���
 */
#define    PTT_ACK_VALID_LEN        100

/**
 * @def  RTP_MAX_DATA_SIZE
 * @brief  �����������س���
 */
#define   RTP_MAX_DATA_SIZE  28
/******************************************************************************
 *   ���Ͷ���
 *   *************************************************************************/
  /******************************************************************************
 *   �ṹ������
 *   *************************************************************************/


/**
 * @struct _CENTER_CMD_SHAREHEADER
 * @brief  ���뵥Ԫ���������������ͷ
 */
typedef struct _CENTER_CMD_SHARE_HEAD
{
    unsigned short SigHead;        // ��ʶAC�ӿ�����ͷ��־(0x13ec)
    unsigned short SigType;        // �������� ��ʶAC�ӿ�PTT�������0x000a��
    unsigned short Datalength;     // ��Ч���ݳ��� ��ʶ������Ч���ݳ��ȣ�100��
    unsigned short SourceID;       // Դ��Ԫ�� ��ʶAC�ӿڷ��͵�ԪIDֵ
    unsigned short DestID;         // Ŀ�ĵ�Ԫ��  ��ʶAC�ӿڽ��յ�ԪIDֵ
    unsigned char  SourceType;     // Դ��Ԫ�豸���� ��ʶ���͵�Ԫ�豸���ͣ���ʽ��
    unsigned char  SigVersion;     // ����汾�� ��ʶ�������ʺϰ汾��0x02(��������0x01�汾������)
    unsigned char  PathNum ;       // ͨ������ ��ʶ���������ڸõ�Ԫ��ͨ�����룬��������ͨ��������ȫF
    unsigned char  Exhead[11];     // ��չͷ ��չͷ����������Ĳ���
    unsigned int   CheckVal;       // У��ֵ ��ʶ����У����(01�汾 ��02�汾 Ŀǰ����дȫF����ʾ��У�鷽ʽ)
} CENTER_CMD_SHARE_HEAD ;



/**
 * @struct  _PTT_CMD
 * @brief  PTT-ON  PTT-OFF  ����ṹ��
 */
typedef struct _PTT_CMD
{
    CENTER_CMD_SHARE_HEAD  SharedHead;
    unsigned int  CallId;            //������ID ��ʶһ�κ��еĿ�����ID
    unsigned int  VoiceId;           //ҵ����ID ��ʶһ�κ��е�ҵ����ID
    unsigned char PttStat;           //PTT����״̬ ��ʶPTT����״̬������CMD_ON=0x06���ͷ�CMD_OFF=0x07��
    unsigned char Reserved;          //�ֽڶ���ռλ����������Ĳ���
    unsigned char TalkingNum[30];    //��ʶ����ͨ��PTT�Ľ���������
    unsigned char CallingNum[30];    //��ʶ���κ��е����к���
    unsigned char CalledNum[30];     //��ʶ���κ��еı��к��루��š����ţ�
} PTT_CMD;



/**
 * @struct   _CENTER_VOICE_DATA_DL
 * @brief ��������֡���нṹ
 */
typedef  struct _CENTER_VOICE_DATA_DL

    unsigned char  Version:2;
    unsigned char  Padding:1;      //�����
    unsigned char  ExtendHead:1;   //��չͷ���
    unsigned char  CsrcCount:4;   //����Դ����
    unsigned char  Marker:1;       //RTP��Ҫ�¼����
    unsigned char  PayloadType:7;  //RTP ��������
    unsigned short SequenceNum ;     //RTP ���к�
    unsigned int   TimeStamp;        //RTPʱ���
    unsigned int   Ssrc;             //RTP ͬ��Դ
    unsigned short ProfileHead;      //rtp��չͷ���Բ���
    unsigned short ExheadLen;        //RTP��չͷ���ݳ���
    unsigned int   VoiceId;          //��ʶһ�κ��е�ҵ����ID
    unsigned int   Gps;              //GPSʱ��
    unsigned char  CodecFlg;         //���ݱ���������ʶ
    unsigned char  FrameFlg;         //����֡��ʶ
    unsigned char  PathNum;          //����ͨ�����룬��������ͨ��������ȫF
    unsigned char  PackRate;         //��ʶ����������
    unsigned char  EncryptFlag;      //��ʶ�Ƿ����
    unsigned char  Reserve[3];
    unsigned char  VoiceData[507];
    unsigned char  ReserveData[5];
 } CENTER_VOICE_DATA_DL;



/**
 * @struct  _Rtp_Fix_Head
 * @brief RTP �̶�ͷ
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
 * @brief RTP ��չͷ
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
 * @brief RTP ת����չ
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
 * @brief �����������ݰ���ʽ
 */
typedef struct _CENTER_VOICE_DATA
{
    Rtp_Fix_Head    Rtp_Head;
    Rtp_Ex_Head     Rtp_Ex;
    unsigned char   Payload[RTP_MAX_DATA_SIZE];
    unsigned char   Reserve_Data[5];
} CENTER_VOICE_DATA;

/******************************************************************************
 *   ȫ�ֱ�������
 *   *************************************************************************/

/******************************************************************************
 *   �ֲ���������
 *   *************************************************************************/

/******************************************************************************
 *   �ڲ�����ʵ��
 *   *************************************************************************/
 /**
 * @brief  ��ʱ��������
 */
void timer_handler (int signo);

/**
* @brief     ��ʱ���ص�����
*/
void timercallback();

/******************************************************************************
 *   �ɵ��ú���ʵ��
 *   *************************************************************************/

#endif //CC_CCLDL_H
























