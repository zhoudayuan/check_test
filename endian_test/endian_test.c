#include <stdio.h>

#define  BIG_ENDIAN         1
#define  LITTLE_ENDIAN      0

static int IsBigEndian();
static unsigned int U32Change2BigEndian(unsigned int data);

int main()
{
    unsigned int data;
    data = 0x11223344;
    printf("1-data=%#x\n", data);
    data=U32Change2BigEndian(data);
    printf("2-data=%#x\n", data);
}




static int IsBigEndian()
{
    union _NUM
    {
        unsigned int  a;
        unsigned char b;
    } num;
    num.a = 0x12345678;
    if (num.b == 0x12)
    {
//  printf("BIG_ENDIAN num.b=%#x\n", num.b);
//  printf("0XFF<<(8*3)=%#x\n", 0XFF<<(8*3));
        return BIG_ENDIAN;
    }
//  printf("LITTLE_ENDIAN num.b=%#x\n", num.b);
//  printf("0XFF<<(8*3)=%#x\n", 0XFF<<(8*3));

    return LITTLE_ENDIAN;
}

static unsigned int U32Change2BigEndian(unsigned int data)
{
    unsigned int val;
    if (IsBigEndian() == LITTLE_ENDIAN)
    {
        val = (\
                  ((data & (0XFF<<(8*3))) >> 8*3) |\
                  ((data & (0XFF<<(8*2))) >> 8*1) |\
                  ((data & (0XFF<<(8*1))) << 8*1) |\
                  ((data & (0XFF<<(8*0))) << 8*3)\
              );
        //printf("@@data & (0XFF<<(8*3)) >> (8*3))=%#x\n", data & (0XFF<<(8*3)) );
        //printf("@@val=%#x\n", val);
    }
    return data;
}
