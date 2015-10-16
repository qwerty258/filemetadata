#include "jsonValueFinder.h"
#include "HexByteConvert.h"
#include <iostream>
#include <vector>
using namespace std;
#include "workingThread.h"

typedef struct _FilenameFilesizeSHA
{
    char* fileName;
    unsigned __int64 fileSize;
    unsigned char SHAValue[SHA_DIGEST_LENGTH];
    unsigned char MD4Value[MD4_DIGEST_LENGTH];
    unsigned char MD5Value[MD5_DIGEST_LENGTH];
    bool compared;
    bool match;
}FilenameFilesizeSHA;

void printHelp(void)
{
    cout << "parameter error\nFileMetadata -jsonfiles files -d directoryFileList\n" << endl;
    exit(0);
}

int main(int argc, char* argv[])
{
    if(5 > argc)
    {
        printHelp();
    }
    vector<char*> filePaths;
    char* directoryName = NULL;
    for(int i = 0; i < argc; i++)
    {
        if(0 == strncmp("-jsonfiles", argv[i], strlen("-jsonfiles")))
        {
            for(int j = i + 1; j < argc; j++, i++)
            {
                if(0 == strncmp("-d", argv[j], strlen("-d")))
                {
                    directoryName = argv[j + 1];
                    break;
                }
                filePaths.push_back(argv[j]);
            }
        }
    }
    if(0 == filePaths.size() || NULL == directoryName)
    {
        printHelp();
    }
    vector<FilenameFilesizeSHA> fileList;
    FILE* pFile = NULL;
    long jsonFileSize = 0;
    char* jsonFileContent = NULL;
    char* pEndPosition = NULL;
    json_value* pJsonValue = NULL;
    json_value* pJsonValuetemp = NULL;
    json_value* pJsonValueInloop = NULL;
    FilenameFilesizeSHA FilenameFilesizeSHATemp = {0};
    for(size_t i = 0; i < filePaths.size(); i++)
    {
        pFile = fopen(filePaths[i], "rb");
        fseek(pFile, 0, SEEK_END);
        jsonFileSize = ftell(pFile);
        rewind(pFile);
        jsonFileContent = (char*)malloc(jsonFileSize);
        fread(jsonFileContent, jsonFileSize, 1, pFile);
        fclose(pFile);
        pJsonValue = json_parse(jsonFileContent, jsonFileSize);
        free(jsonFileContent);

        pJsonValuetemp = jsonObjectFinder(pJsonValue, "data");
        if(NULL != pJsonValuetemp)
        {
            if(json_array == pJsonValuetemp->type)
            {
                for(unsigned int i = 0; i < pJsonValuetemp->u.array.length; i++)
                {
                    pJsonValueInloop = jsonObjectFinder(pJsonValuetemp->u.array.values[i], "n");
                    FilenameFilesizeSHATemp.fileName = jsonStringDup(pJsonValueInloop);

                    pJsonValueInloop = jsonObjectFinder(pJsonValuetemp->u.array.values[i], "s");
                    if(NULL != pJsonValueInloop && json_integer == pJsonValueInloop->type)
                    {
                        FilenameFilesizeSHATemp.fileSize = pJsonValueInloop->u.integer;
                    }

                    pJsonValueInloop = jsonObjectFinder(pJsonValuetemp->u.array.values[i], "sha");
                    jsonFileContent = jsonStringDup(pJsonValueInloop);
                    Hex2Byte(jsonFileContent, FilenameFilesizeSHATemp.SHAValue, SHA_DIGEST_LENGTH);
                    free(jsonFileContent);

                    FilenameFilesizeSHATemp.compared = false;
                    FilenameFilesizeSHATemp.match = false;

                    fileList.push_back(FilenameFilesizeSHATemp);
                }
            }
        }

        json_value_free(pJsonValue);

        pJsonValue = NULL;
        pJsonValuetemp = NULL;
        pJsonValueInloop = NULL;
        jsonFileContent = NULL;
        pFile = NULL;
        jsonFileSize = 0;
    }

    pFile = fopen(directoryName, "rb");
    fseek(pFile, 0, SEEK_END);
    jsonFileSize = ftell(pFile);
    rewind(pFile);
    jsonFileContent = (char*)calloc(1, jsonFileSize + 5);
    fread(jsonFileContent, jsonFileSize, 1, pFile);
    fclose(pFile);

    filePaths.clear();
    size_t currentIndex = 0;
    while(currentIndex < jsonFileSize)
    {
        filePaths.push_back(jsonFileContent + currentIndex);
        while(jsonFileContent[currentIndex] != '\r' && jsonFileContent[currentIndex] != '\n'&&jsonFileContent[currentIndex] != '\0')
        {
            currentIndex++;
        }
        if(jsonFileContent[currentIndex] == '\r')
        {
            jsonFileContent[currentIndex] = '\0';
            currentIndex++;
        }
        if(jsonFileContent[currentIndex] == '\n')
        {
            jsonFileContent[currentIndex] = '\0';
            currentIndex++;
        }
    }

    HANDLE threadHandles[4];
    threadPamater* threadPamaterPointers[4];
    for(size_t i = 0; i < 4; i++)
    {
        threadPamaterPointers[i] = (threadPamater*)malloc(sizeof(threadPamater));
    }
    size_t threadCount;
    size_t i = 0;
    size_t j = 0;
    DWORD threadID;
    while(i < fileList.size())
    {
        threadCount = 0;
        for(i = 0; i < fileList.size(); i++)
        {
            if(fileList[i].compared)
            {
                continue;
            }
            for(j = 0; j < filePaths.size(); j++)
            {
                if(NULL != strstr(filePaths[j], fileList[i].fileName))
                {
                    threadPamaterPointers[threadCount]->compared = &(fileList[i].compared);
                    threadPamaterPointers[threadCount]->filePath = filePaths[j];
                    threadPamaterPointers[threadCount]->match = &(fileList[i].match);
                    threadPamaterPointers[threadCount]->MD4Value = fileList[i].MD4Value;
                    threadPamaterPointers[threadCount]->MD5Value = fileList[i].MD5Value;
                    threadPamaterPointers[threadCount]->SHAValue = fileList[i].SHAValue;
                    threadPamaterPointers[threadCount]->size = &(fileList[i].fileSize);
                    threadHandles[threadCount] = CreateThread(NULL, 0, SHACalculator, threadPamaterPointers[threadCount], 0, &threadID);
                    threadCount++;
                    break;
                }
            }
            if(4 <= threadCount)
            {
                break;
            }
        }
        WaitForMultipleObjects(4, threadHandles, TRUE, INFINITE);
    }

    for(size_t i = 0; i < 4; i++)
    {
        free(threadPamaterPointers[i]);
    }

    pFile = fopen("result.txt", "wb");
    char* resultBuffer = (char*)malloc(2048);
    char* SHA1String;
    char* MD5String;
    char* MD4String;
    if(NULL != pFile && NULL != resultBuffer)
    {
        for(i = 0; i < fileList.size(); i++)
        {
            memset(resultBuffer, 0x0, 2048);
            SHA1String = Byte2Hex(fileList[i].SHAValue, SHA_DIGEST_LENGTH);
            MD5String = Byte2Hex(fileList[i].MD5Value, MD5_DIGEST_LENGTH);
            MD4String = Byte2Hex(fileList[i].MD4Value, MD4_DIGEST_LENGTH);
            sprintf(resultBuffer, "%s, SHA1:%s, MD5:%s, MD4:%s\n", fileList[i].fileName, SHA1String, MD5String, MD4String);
            fwrite(resultBuffer, strlen(resultBuffer), 1, pFile);
            free(SHA1String);
            free(MD5String);
            free(MD4String);
        }
        fclose(pFile);
        free(resultBuffer);
    }

    free(jsonFileContent);

    for(size_t i = 0; i < fileList.size(); i++)
    {
        free(fileList[i].fileName);
    }
    fileList.clear();

    return 0;
}

