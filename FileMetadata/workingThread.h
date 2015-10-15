#pragma once

#include <openssl\sha.h>
#include <openssl\md4.h>
#include <openssl\md5.h>
#include <Windows.h>

typedef struct _threadPamater
{
    char* filePath;
    unsigned char* SHAValue;
    unsigned char* MD4Value;
    unsigned char* MD5Value;
    unsigned __int64* size;
    bool* compared;
    bool* match;
}threadPamater;

DWORD WINAPI SHACalculator(LPVOID lpParam);