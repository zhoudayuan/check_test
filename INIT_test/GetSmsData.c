#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OK                          0
#define NO_INPUT                    1
#define TOO_LONG                    2

#define GET_SUCCEED                 0

#define TOO_LONG_ERR                -1
#define NO_INPUT_ERR                -2
#define NOT_DIGIT_ERR               -3
#define CR_ERR                      -4




int getLine(const char *prmpt, char *buff, int sz)
{
    int ch, extra;

    if (prmpt != NULL)
    {
        // 显示操作提示
        printf("%s", prmpt);
        fflush(stdout);
    }

    // 字符串输入
    if (fgets(buff, sz, stdin) == NULL)
    {
        printf("NO_INPUT\n");
        return NO_INPUT;
    }

//   printf("[%s:%d]buff=(%s), sz=(%d), strlen(buff)=(%d)\n", __FUNCTION__, __LINE__, buff, sz, strlen(buff));

    // 判断buff中是否将输入字符全部存储
    if (buff[strlen(buff)-1] != '\n')
    {
        // 没有全部存储
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
        {
//            printf("[%s:%d]buff=(%s), sz=(%d), strlen(buff)=(%d)\n", __FUNCTION__, __LINE__, buff, sz, strlen(buff));

            extra = 1;
        }
        // 返回 TOO_LONG 说明输入的数据超过缓存长度
//        printf("[%s:%d]buff=(%s), sz=(%d), strlen(buff)=(%d)\n", __FUNCTION__, __LINE__, buff, sz, strlen(buff));
        return (extra == 1) ? TOO_LONG : OK;
    }
    // 全部存储, 将'\n'替换为'\0'
//    printf("[%s:%d]buff=(%s), sz=(%d), strlen(buff)=(%d)\n", __FUNCTION__, __LINE__, buff, sz, strlen(buff));
    buff[strlen(buff)-1] = '\0';
    return OK;
}







#define MSG_GET_OK				 0
#define MSG_TOO_LONG_ERR		-1
#define MSG_NO_INPUT_ERR		-2

int Msg_Get(const char *prmpt, char *buff, int sz)
{
	int ret;

	ret = getLine(prmpt, buff, sz);
	if (ret == TOO_LONG)
	{
	    printf("\n\nInput MSG Is Too Long, Please Try Again\n\n");
	    return MSG_TOO_LONG_ERR;
	}
    else if (ret == NO_INPUT)
	{
		printf ("\n\nNo Input, Please Try Again\n\n");
		return MSG_NO_INPUT_ERR;
	}
	else if (ret == OK)
	{
		return MSG_GET_OK;
	}
	return MSG_GET_OK;
	
}






void SetSmsData(char *pSmsBuff, int len)
{
	int buff_len, i;
	char buff[100];

	if (sizeof(buff)/2 > len)
	{
		printf("Set sms len is err:\n\n");
		exit(0);
	}
	
	

	while (1)
	{
		printf("Set sms, please input:\n\n");
		if (Msg_Get("sms data>", buff, sizeof(buff)) == MSG_GET_OK)
		{
			break;
		}
	}
	
	buff_len = strlen(buff);
	
	if (buff_len > len/2)
	{
		printf("Set sms len is too long\n\n");
		exit(0);

	}

	memset(pSmsBuff, 0, len);

	for (i = 0; i < buff_len; i++)
	{
		pSmsBuff[i*2] = buff[i];
	}
}



int main(int argc, char *argv[])
{
	int i;
	char sms_data[200];
	sms_data[0] = 'h';
    sms_data[1] = 0x00;
    sms_data[2] = 'y';
    sms_data[3] = 0x00;
    sms_data[4] = 't';
    sms_data[5] = 0x00;
    sms_data[6] = 'e';
    sms_data[7] = 0x00;
    sms_data[8] = 'r';
    sms_data[9] = 0x00;
    sms_data[10] = 'a';
    sms_data[11] = 0x00;
    sms_data[12] = '1';
    sms_data[13] = 0x00;
    sms_data[14] = '2';
    sms_data[15] = 0x00;
    sms_data[16] = '3';
    sms_data[17] = 0x00;
    SetSmsData(&sms_data[18], 200-18);
    
    for (i = 0; i < (200-18); i++)
    {
		printf("%d=%c(%d)\n", i+18, sms_data[i+18], sms_data[i+18]);
	}
	
	
	printf("\n\n~~~~~~~~~~~~~~~~~~~~~~\n\n\n\n\n");

    for (i = 0; i < (200); i++)
    {
		printf("%d=%c(%d)\n", i, sms_data[i], sms_data[i]);
	}
    
	printf("\n[%s:%s:%d]\n\n", __FILE__, __FUNCTION__, __LINE__);
	
    return 0;
}



