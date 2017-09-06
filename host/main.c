/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <err.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* OP-TEE TEE client API (built by optee_client) */
#include <tee_client_api.h>

/* To the the UUID (found the the TA's h-file(s)) */
#include "my_test_ca.h"



static int g_TaskInitFlag = -1;    /* Flag if the task done initialize operation */
TEEC_UUID svc_id = TA_MY_TEST_UUID;
TEEC_Context g_TaskContext;
CHAR g_RandomOut[512] = {0};
/* Buffer for sha operation */
CHAR g_ShaTestBuf[] ={
     'Y', 'o', 'u', ' ', 'y', 'o', 'u', ' ', 'c', 'h', 'e', 'c', 'k', ' ', 'n', 'o',
     'w', 'j', 'i', 'a', 'n', ' ', 'b', 'i', 'n', 'g', ' ', 'g', 'u', 'o', ' ', 'z',
     'i', ' ', 'l', 'a', 'i', ' ', 'y', 'i', ' ', 't', 'a', 'o', '!', '!', '!', '!'};
CHAR g_ShaOutput[80] = {0};

CHAR g_Sha1Result[] = 
{   
    0x21, 0x9b, 0x5b, 0x8b, 0x25, 0x6f, 0x0e, 0x52, 0xcb, 0x2f, 0xfe, 0xfd, 0x6c, 0x47, 0xd7, 0xb4, 
    0x44, 0x00, 0x57, 0xc3
};


CHAR g_Sha256Result[] = 
{   
    0xda, 0x52, 0xe9, 0xc2, 0x53, 0xae, 0x03, 0x30, 0xbd, 0x97, 0x3f, 0xa5, 0xf3, 0xea, 0x51, 0x1d, 
    0x31, 0x0a, 0xdf, 0x1f, 0x0a, 0xc0, 0x0e, 0x62, 0x0f, 0x2d, 0x5e, 0x99, 0xf5, 0xc8, 0x6b, 0x8f
};






void g_CA_PrintfBuffer(CHAR* buf, UINT32 len)
{
    UINT32 index = 0U;
    for(index = 0U; index < len; index++)
    {
        if(index < 15U)
        {
        }
        else if(0U == index%16U)
        {
            printf("\n");
            
        }
        else
        {
        }
        
        printf("0x%02x, ", (buf[index] & 0x000000FFU));
        

    }
    printf("\n");
   
}


int l_CryptoVerifyCa_TaskInit(void)
{
    TEEC_Result result;
    int l_RetVal = OK;
    
    /**1) Check if need to do task initialization operation */
    if(-1 == g_TaskInitFlag)
    {
        result = TEEC_InitializeContext(NULL, &g_TaskContext);
        if(result != TEEC_SUCCESS) 
        {
            printf("InitializeContext failed, ReturnCode=0x%x\n", result);
            l_RetVal= FAIL;
        } 
        else 
        {
            g_TaskInitFlag = 0;
            printf("InitializeContext success\n");
            l_RetVal = OK;
        }
    }
    
    return l_RetVal;
}


int l_CryptoVerifyCa_OpenSession(TEEC_Session* session)
{
    TEEC_Result result;
    int l_RetVal = FAIL;
    uint32_t origin;

    result = TEEC_OpenSession(&g_TaskContext, session, &svc_id, 
                                TEEC_LOGIN_PUBLIC, NULL, NULL, &origin);
    if(result != TEEC_SUCCESS) 
    {
        printf("OpenSession failed, ReturnCode=0x%x, ReturnOrigin=0x%x\n", result, origin);
        g_TaskInitFlag = -1;
        l_RetVal = FAIL;
    } 
    else 
    {
        printf("OpenSession success\n");
        l_RetVal = OK;
    }

    return l_RetVal;
}


int l_CryptoVerifyCa_SendCommand(TEEC_Operation* operation, TEEC_Session* session, uint32_t commandID)
{
    TEEC_Result result;
    int l_RetVal = FAIL;
    uint32_t origin;

    result = TEEC_InvokeCommand(session, commandID, operation, &origin);
    if (result != TEEC_SUCCESS) 
    {
        printf("InvokeCommand failed, ReturnCode=0x%x, ReturnOrigin=0x%x\n", result, origin);
        l_RetVal = FAIL;
    } 
    else 
    {
        printf("InvokeCommand success\n");
        l_RetVal = OK;
    }


    return l_RetVal;
}


void g_CryptoVerifyCa_Helloworld(void)
{
    TEEC_Session   l_session;    /* Define the session of TA&CA */
    TEEC_Operation l_operation;  /* Define the operation for communicating between TA&CA */
    int l_RetVal = FAIL;       /* Define the return value of function */

    /**1) Initialize this task */
    l_RetVal = l_CryptoVerifyCa_TaskInit();
    if(FAIL == l_RetVal)
    {
        goto cleanup_1;
    }

    /**2) Open session */
    l_RetVal = l_CryptoVerifyCa_OpenSession(&l_session);
    if(FAIL == l_RetVal)
    {
        goto cleanup_2;
    }

    /* Clear the TEEC_Operation struct */
    memset(&l_operation, 0, sizeof(TEEC_Operation));

    /*
     * Prepare the argument. Pass a value in the first parameter,
     * the remaining three parameters are unused.
     */
    l_operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_NONE,
                     TEEC_NONE, TEEC_NONE);
    l_operation.params[0].value.a = 42;

    /**4) Send command to TA */
    l_RetVal = l_CryptoVerifyCa_SendCommand(&l_operation, &l_session, TA_MY_TEST_CMD_INC_VALUE);
    if(FAIL == l_RetVal)
    {
        goto cleanup_3;
    }

    /**5) The clean up operation */
    cleanup_3:
        TEEC_CloseSession(&l_session);
    cleanup_2:
        TEEC_FinalizeContext(&g_TaskContext);
    cleanup_1:
        printf("over\n");

}

int g_CryptoVerifyCa_Random(UINT32 len, CHAR* output)
{
    TEEC_Session   l_session;    /* Define the session of TA&CA */
    TEEC_Operation l_operation;  /* Define the operation for communicating between TA&CA */
    int l_RetVal = FAIL;       /* Define the return value of function */

    /**1) Initialize this task */
    l_RetVal = l_CryptoVerifyCa_TaskInit();
    if(FAIL == l_RetVal)
    {
        goto cleanup_1;
    }

    /**2) Open session */
    l_RetVal = l_CryptoVerifyCa_OpenSession(&l_session);
    if(FAIL == l_RetVal)
    {
        goto cleanup_2;
    }

    /**3) Set the communication context between CA&TA */
    memset(&l_operation, 0x0, sizeof(TEEC_Operation));
    l_operation.started = 1;
    l_operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,TEEC_NONE, 
                                              TEEC_NONE, TEEC_NONE);
    l_operation.params[0].tmpref.size = len;
    l_operation.params[0].tmpref.buffer = output;

    /**4) Send command to TA */
    l_RetVal = l_CryptoVerifyCa_SendCommand(&l_operation, &l_session, TA_MY_TEST_CMD_RANDOM);
    if(FAIL == l_RetVal)
    {
        goto cleanup_3;
    }

    /**5) The clean up operation */
    cleanup_3:
        TEEC_CloseSession(&l_session);
    cleanup_2:
        TEEC_FinalizeContext(&g_TaskContext);
    cleanup_1:
        return l_RetVal;
}




int g_CryptoVerifyCa_Sha(CHAR* pData, UINT32 len, EN_SHA_MODE shaMode, CHAR* output, UINT32 outLen)
{
    TEEC_Session   l_session;    /* Define the session of TA&CA */
    TEEC_Operation l_operation;  /* Define the operation for communicating between TA&CA */
    int l_RetVal = FAIL;       /* Define the return value of function */

    /**1) Initialize this task */
    l_RetVal = l_CryptoVerifyCa_TaskInit();
    if(FAIL == l_RetVal)
    {
        goto cleanup_1;
    }

    /**2) Open session */
    l_RetVal = l_CryptoVerifyCa_OpenSession(&l_session);
    if(FAIL == l_RetVal)
    {
        goto cleanup_2;
    }

    /**3) Set the communication context between CA&TA */
    memset(&l_operation, 0x0, sizeof(TEEC_Operation));
    l_operation.started = 1;
    l_operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,TEEC_VALUE_INPUT, 
                                              TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE);
    l_operation.params[0].tmpref.size = len;
    l_operation.params[0].tmpref.buffer = pData;
    l_operation.params[1].value.a = shaMode;
    l_operation.params[2].tmpref.size = outLen;
    l_operation.params[2].tmpref.buffer = output;

    /**4) Send command to TA */
    l_RetVal = l_CryptoVerifyCa_SendCommand(&l_operation, &l_session, TA_MY_TEST_CMD_HASH);
    printf("The respond data length is 0x%02x\n", outLen);
    if(FAIL == l_RetVal)
    {
        goto cleanup_3;
    }

    /**5) The clean up operation */
    cleanup_3:
        TEEC_CloseSession(&l_session);
    cleanup_2:
        TEEC_FinalizeContext(&g_TaskContext);
    cleanup_1:
        return l_RetVal;
}







int main(int argc, char *argv[])
{
    
    if(0 == memcmp(argv[1], "helloworld", 10))
    {
        printf("Entry get helloworld CA\n");
        g_CryptoVerifyCa_Helloworld();
        printf("The Respond helloworld from TA just like follow:\n");
    }

    if(0 == memcmp(argv[1], "sha1", 4))
    {
        printf("Entry sha1 CA\n");
        g_CryptoVerifyCa_Sha(g_ShaTestBuf, sizeof(g_ShaTestBuf), EN_OP_SHA1, g_ShaOutput, 20);
        printf("The Respond hash data from TA just like follow:\n");
        g_CA_PrintfBuffer(g_ShaOutput, 20);
    }



    if(0 == memcmp(argv[1], "sha256", 6))
    {

        printf("Entry sha256 CA\n");
        g_CryptoVerifyCa_Sha(g_ShaTestBuf, sizeof(g_ShaTestBuf), EN_OP_SHA256, g_ShaOutput, 32);
        printf("The Respond hash data from TA just like follow:\n");
        g_CA_PrintfBuffer(g_ShaOutput, 32);
    }
    

    return 0;
}



