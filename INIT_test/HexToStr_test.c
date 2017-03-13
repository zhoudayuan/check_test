


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "typedef.h"
 

/******************************************************************************
 *   可调用函数实现
 *   *************************************************************************/
/**
 * @brief   将16进制无符号数转换成字符串
 *
 * @param [out] puData     转换后的字符串
 * @param [in]  u4Data     转换前的16进制无符号数
 * @author  柳智奇
 * @since   trunk.00001
 * @bug
 */
void CLI_HexToStr(UINT8* puData, UINT32 u4Data)
{


    INT32 i4Temp;

    if (NULL == puData)
    {
        return;
    }

    puData[8] = 0;

    for (i4Temp = 7; i4Temp >= 0; i4Temp--, u4Data >>= 4)
    {
        if ((u4Data & 0x000f) <= 9)
        {
            puData[i4Temp] = (u4Data & 0x000f) + '0';
        }
        else
        {
            puData[i4Temp] = (u4Data & 0x000f) + 'a' - 0x0a;
        }
    }
    return;
}



int test_CLI_HexToStr(argc, argv)
{
	
	UINT8 Data[30] = {0};
	UINT32 u4Data=0x12345678;
	
	CLI_HexToStr(Data, u4Data);
	printf("Data=%s\n", Data);
	return 0;
}


int main(int argc, char *argv[])
{

    test_CLI_HexToStr(argc, argv);
    return 0;
}
