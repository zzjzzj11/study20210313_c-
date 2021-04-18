/*
 * @Author: your name
 * @Date: 2021-04-17 11:43:03
 * @LastEditTime: 2021-04-17 16:21:01
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /code_fragment/byteorder.cpp
 */
#include <stdio.h>

void byteorder()
{
    union
    {   
        short value;
        char  union_bytes[sizeof(short)];
    } test;
    test.value = 0x0102;

    if ((test.union_bytes[0] == 1) && (test.union_bytes[1] == 2 ))
    {
        printf("big endian\n");

    }
    else if(test.union_bytes[0] == 2 && test.union_bytes[1] == 1 )
    {
        printf("little endian\n");
    }
    else
    {
        printf("unknown ...\n");
    }

}

int main()
{
    byteorder();
    return 0;
}
