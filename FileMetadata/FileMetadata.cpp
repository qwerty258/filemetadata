#include "jsonValueFinder.h"
#include "HexByteConvert.h"
#include <iostream>
#include <vector>
using namespace std;

typedef struct _FilenameFilesizeSHA
{
    char* fileName;
    __int64 fileSize;
    unsigned char SHAValue[20];
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
                    Hex2Byte(jsonFileContent, FilenameFilesizeSHATemp.SHAValue, 20);
                    free(jsonFileContent);

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

    free(jsonFileContent);


    for(size_t i = 0; i < fileList.size(); i++)
    {
        free(fileList[i].fileName);
    }
    fileList.clear();

    return 0;
}

