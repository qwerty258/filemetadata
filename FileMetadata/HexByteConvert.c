#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void Hex2Byte(const char* hexString, unsigned char* byteArray, size_t byteArraySize)
{
    char temp[3];
    char* end;
    for(size_t i = 0; i < byteArraySize; i++)
    {
        temp[0] = hexString[i * 2];
        temp[1] = hexString[i * 2 + 1];
        temp[2] = 0;
        byteArray[i] = (unsigned char)strtoul(temp, &end, 16);
    }
}

char* Byte2Hex(unsigned char* byteArray, size_t byteArraySize)
{
    char temp[3];
    char* hexString = (char*)calloc(1, byteArraySize * 2 + 1);
    if(NULL != hexString)
    {
        for(size_t i = 0; i < byteArraySize; i++)
        {
            sprintf(temp, "%X", byteArray[i]);
            strcat(hexString, temp);
        }
    }
    return hexString;
}