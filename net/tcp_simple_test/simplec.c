#if 0


#include <arpa/inet.h>
struct sockaddr_in {
　　short int sin_family;           /* Address family */
　　unsigned short int sin_port;    /* Port number */
　　struct in_addr sin_addr;        /* Internet address */
　　unsigned char sin_zero[8];      /* Same size as struct sockaddr */
　　};

　　sin_family：    指代协议族，在socket编程中只能是 AF_INET
　　sin_port：      存储端口号（使用网络字节顺序）
　　sin_addr：      存储IP地址，使用in_addr这个数据结构
　　sin_zero：      是为了让sockaddr与sockaddr_in两个数据结构保持大小相同而保留的空字节。

    struct in_addr
    {
        union
        {
            struct
            {
                u_char s_b1,s_b2,s_b3,s_b4;
            } S_un_b; //An IPv4 address formatted as four u_chars.
            struct
            {
                u_short s_w1,s_w2;
            } S_un_w; //An IPv4 address formatted as two u_shorts
           u_long S_addr;//An IPv4 address formatted as a u_long
        } S_un;

    #define s_addr S_un.S_addr
};

#endif




#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in peer;
    int s;
    int rc;
    char buf[1];

    // 初始化地址
    bzero(&peer, sizeof(peer));
    peer.sin_family = AF_INET;
    peer.sin_port = htons(7500);
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
    {
        perror("socket call failed>");
        exit(1);
    }


    rc = connect(s, (struct sockaddr *)&peer, sizeof(peer));
    if (rc  < 0)
    {
        perror("connect call failed>");
        exit(1);
    }


    rc = send(s, "1", 1, 0);
    if (rc  < 0)
    {
        perror("send call failed>");
        exit(1);
    }

    rc = recv(s, buf, 1, 0);
    
    if (rc <= 0)
    {
        perror("recv call failed>");
    }
    else
    {
        printf("\n%c\n\n", buf[0]);
    }
    exit(0);
}

