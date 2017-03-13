/******************************************************************************
 * Copyright(C), 1993-2009, HYT
 * File name: odp_internal.h
 * Author: zhiqi.liu    Version:  0.1alpha      Date: 2009-07-23
 * Description:
 *     ���ļ�����ƽ̨��ض���
 * Others:
 *
 * History:
 * 1. Date:
 * Author:
 * Modification:
******************************************************************************/

#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

/** @defgroup systype 1. ƽ̨��ض��� */
/** @{ */
#define VOID           void

/** @defgroup �������Ͷ��� */
/** @{ */
#define NO_ERR                      (0)     ///< ���ݳɹ�
#define ERR                         (1)     ///< ���ݴ���


#ifndef NULL
#define NULL           ((void*)0)
#endif

#ifndef BOOL
typedef int            BOOL;
#endif
#ifndef TRUE
#define TRUE           ((BOOL) 1)
#endif
#ifndef FALSE
#define FALSE          ((BOOL) 0)
#endif

typedef unsigned long  UINT32;
typedef signed long    INT32;
typedef unsigned short UINT16;
typedef signed short   INT16;
typedef unsigned char  UINT8;
typedef signed char    INT8;
typedef char           CHAR;

typedef int            STATUS;
typedef void*          HANDLE_T;
/** @} */

#endif /* _TYPEDEF_H_ */

