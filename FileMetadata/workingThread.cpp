#include "workingThread.h"
#include <stdio.h>
#define READSIZE 1024*1024*4
DWORD WINAPI SHACalculator(LPVOID lpParam)
{
    threadPamater* pThreadPamater = static_cast<threadPamater*>(lpParam);
    SHA_CTX* pSHA_CTX = (SHA_CTX*)malloc(sizeof(SHA_CTX));
    MD4_CTX* pMD4_CTX = (MD4_CTX*)malloc(sizeof(MD4_CTX));
    MD5_CTX* pMD5_CTX = (MD5_CTX*)malloc(sizeof(MD5_CTX));
    unsigned char SHADigestResult[SHA_DIGEST_LENGTH];
    unsigned char MD4DigestResult[MD4_DIGEST_LENGTH];
    unsigned char MD5DigestResult[MD5_DIGEST_LENGTH];
    unsigned __int64 readSize = READSIZE;
    unsigned __int64 totalSize = 0;
    char* pBuffer = (char*)malloc(READSIZE);
    FILE* pFile = fopen(pThreadPamater->filePath, "rb");
    if(NULL != pFile)
    {
        if(SHA1_Init(pSHA_CTX))
        {
            if(MD4_Init(pMD4_CTX))
            {
                if(MD5_Init(pMD5_CTX))
                {
                    while(READSIZE <= readSize)
                    {
                        readSize = fread(pBuffer, 1, READSIZE, pFile);
                        SHA1_Update(pSHA_CTX, pBuffer, readSize);
                        MD4_Update(pMD4_CTX, pBuffer, readSize);
                        MD5_Update(pMD5_CTX, pBuffer, readSize);
                        totalSize += readSize;
                    }

                    SHA1_Final(SHADigestResult, pSHA_CTX);
                    MD4_Final(MD4DigestResult, pMD4_CTX);
                    MD5_Final(MD5DigestResult, pMD5_CTX);

                    (*pThreadPamater->compared) = true;

                    if(0 == memcmp(SHADigestResult, pThreadPamater->SHAValue, SHA_DIGEST_LENGTH))
                    {
                        (*pThreadPamater->match) = true;
                    }
                    else
                    {
                        (*pThreadPamater->match) = false;
                    }

                    if(totalSize != (*pThreadPamater->size))
                    {
                        (*pThreadPamater->match) = false;
                    }

                    if((*pThreadPamater->match))
                    {
                        memcpy(pThreadPamater->MD4Value, MD4DigestResult, MD4_DIGEST_LENGTH);
                        memcpy(pThreadPamater->MD5Value, MD5DigestResult, MD5_DIGEST_LENGTH);
                    }
                }
            }
        }
    }
    fclose(pFile);
    free(pSHA_CTX);
    free(pMD4_CTX);
    free(pMD5_CTX);
    free(pBuffer);


    return 0;
}