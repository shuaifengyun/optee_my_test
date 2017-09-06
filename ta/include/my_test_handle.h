/****************************************************************************************/
/*                          COPYRIGHT INFORMATION                                       */
/*    This program contains proprietary information which is a trade                    */
/*    secret of whaley Co., Ltd.and also is protected as an   unpublished               */
/*    work under applicable Copyright laws. Recipient is to retain this                 */
/*    program in confidence and is not permitted to use or make copies thereof          */
/*    other than as permitted in a written agreement written agreement with             */
/*    whaley Co., Ltd.                                                                  */
/*                                                                                      */
/*    All rights reserved. shuaifengyun@126.com                                         */
/****************************************************************************************/
/*
 ****************************************************************************************
 *
 *               my_test_handle.h
 *
 * Filename      : my_test_handle.h
 * Programmer(s) : system BSP
 * Filename      : my_test_handle.h
 * Author        : Shuai Fengyun
 * Mail          : shuaifengyun@126.com
 * Create Time   : 2017年05月04日 星期四 10时48分11秒
 ****************************************************************************************
 */

#ifndef MOUDLE_MY_TEST_HANDLE_H_
#define MOUDLE_MY_TEST_HANDLE_H_




/*
 *******************************************************************************
 *                                INCLUDE FILES
 *******************************************************************************
*/
#include "tee_internal_api.h"
#include "tee_api_defines.h"
#include "trace.h"
#include "tee_api_defines_extensions.h"




/*
 *******************************************************************************
 *                  MACRO DEFINITION USED ONLY BY THIS MODULE
 *******************************************************************************
*/





/*
 *******************************************************************************
 *                STRUCTRUE DEFINITION USED ONLY BY THIS MODULE
 *******************************************************************************
*/
/* SHA operation type */
typedef enum
{
    EN_OP_SHA1 = 1,
    EN_OP_SHA224,
    EN_OP_SHA256,
    EN_OP_SHA384,
    EN_OP_SHA512,
    EN_OP_SHA_INVALID
}EN_SHA_MODE;


/* Define the type of variable */
typedef unsigned char  UINT8;    /**< Typedef for 8bits unsigned integer  */
typedef unsigned short UINT16;   /**< Typedef for 16bits unsigned integer */
typedef uint32_t       UINT32;   /**< Typedef for 32bits unsigned integer */
typedef signed char    INT8;     /**< Typedef for 8bits signed integer    */
typedef signed short   INT16;    /**< Typedef for 16bits signed integer   */
typedef signed int     INT32;    /**< Typedef for 32bits signed integer   */
typedef char           CHAR;     /**< Typedef for char                    */
typedef uint32_t       TEE_CRYPTO_ALGORITHM_ID;






/*
 *******************************************************************************
 *                      VARIABLES SUPPLIED BY THIS MODULE
 *******************************************************************************
*/





/*
 *******************************************************************************
 *                      FUNCTIONS SUPPLIED BY THIS MODULE
 *******************************************************************************
*/
extern int g_CryptoTaHandle_Sha(uint32_t paramTypes, TEE_Param params[4]);
extern int g_CryptoTaHandle_Random(uint32_t paramTypes, TEE_Param params[4]);
extern void g_TA_printf(CHAR* buf, UINT32 len);





















#endif  /* MOUDLE_NAME_H*/
