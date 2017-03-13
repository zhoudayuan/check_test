
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h> 
#include <sys/sem.h>
#include <sys/time.h>
#include <sys/ipc.h>      //shmget
#include <sys/msg.h>      //msgget
#include <sys/shm.h>      //shmget
#include <arpa/inet.h>    //inet_addr(); inet_ntoa()
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <errno.h>

#include "typedef.h"
#include "mgr_common.h"


/**
 * @def  DL
 * @brief ����
 */
#define DL                       "-d"
/**
 * @def  UL
 * @brief ���� 
 */
#define UL                       "-u"

/** @defgroup NAS������id */
/** @{ */
#define	SHORT_MESSAGE            0		///< ����Ϣ
#define	NAS_GPS_REPORT           1		///< NAS GPS ����		  
#define	MS_GPS_REPORT            2		///< MS GPS ����		
#define	NEARID_INFO_QUERY        3		///< �ٵ���Ϣ��ѯ		
#define	NAS_DISABLE_IDT          4		///< NAS ң��ָʾ	
#define	MS_DISABLE_IDT           5		///< MS ң��ָʾ	
#define	NAS_ENABLE_IDT           6		///< NAS ����ָʾ	
#define	MS_ENABLE_IDT            7		///< MS ����ָʾ
#define	NAS_STUN_IDT             8		///< NAS ң��ָʾ


typedef  enum  _SMS_DATA_CALL_TYPE
{ 
    //����Ϣ
    MESSAGE_PRIVATE_CALL    =0x00,    //����
    MESSAGE_GROUP_CALL      =0x01,    //���
    STATUS_PRIVATE_CALL     =0x02,    //״̬����
    STATUS_GROUP_CALL       =0x03,    //״̬���
   
    //����
    STUN_REQ_MS             =0x07,    //ң��MS
    STUN_REQ_NAS            =0x08,    //ң��NAS
    KILL_REQ_NAS            =0x09,    //ң��NAS
    GPS_REPOTR_MS           =0x0a,    //��GPS
    GPS_REPORT_NAS          =0x0b,
    REVIVE_REQ_NAS          =0x0c,
    REVIVE_REQ_MS           =0x0d, 
    NEGHR_QUERY             =0x0e,     //�ڵ��ѯ

    //��Ӧ
    NEGHR_QUERY_ACK         =0x20,     //�ڵ��ѯ��Ӧ   sms_send ��ʽ�ظ�
    GPS_REPROT_MS_ACK       =0x21,     //��Ӧ��MS  GPS       CCL �Ȼ�ACK��ʶ�յ�����           Ȼ��  sms_send ��ʽ�ظ�������Ϣ
    GPS_REPROT_NAS_ACK      =0x22,     //��Ӧ��NAS  GPS      CCL �Ȼ�ACK��ʶ�յ�����     sms_send ��ʽ�ظ�
    STUN_REQ_MS_ACK         =0x23,      
    STUN_REQ_NAS_ACK        =0x24,
    KILL_REQ_NAS_ACK        =0X25,
    REVIVE_REQ_NAS_ACK      =0x26,     //����NAS
    REVIVE_REQ_MS_ACK       =0x27,     //����MS

    //�澯
    DISCON_ALARM            =0x30,     //����NAS    //DISCON_ALARM            =0x13,                       //�����澯
    DISCON_ALARM_CLEAR      =0x31,     //�����澯���
    //�����ٵ��ϱ�
   
}SMS_DATA_CALL_TYPE;



/**
 * @struct MSG_DATA_DL_T
 * @brief ����������Ч���ݽṹ��
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
 * @brief �������е����п��Ʋ�ṹ��
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


