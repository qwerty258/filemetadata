#include "jsonValueFinder.h"
#include "HexByteConvert.h"
#include <yajl\yajl_version.h>
#include <yajl\yajl_parse.h>
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
    cout << "parameter error\nFileMetadata -jsonfiles files -d directory\n" << endl;
    exit(0);
}

int main(int argc, char* argv[])
{
    if(5 > argc)
    {
        printHelp();
    }
    vector<char*> jsonfiles;
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
                jsonfiles.push_back(argv[j]);
            }
        }
    }
    if(0 == jsonfiles.size() || NULL == directoryName)
    {
        printHelp();
    }

    cout << yajl_version() << endl;

    vector<FilenameFilesizeSHA> fileList;
    FILE* pJsonFile = NULL;
    long jsonFileSize = 0;
    char* jsonFileContent = NULL;
    char* pEndPosition = NULL;
    for(size_t i = 0; i < jsonfiles.size(); i++)
    {
        pJsonFile = fopen(jsonfiles[i], "rb");
        fseek(pJsonFile, 0, SEEK_END);
        jsonFileSize = ftell(pJsonFile);
        rewind(pJsonFile);
        jsonFileContent = (char*)malloc(jsonFileSize);
        fread(jsonFileContent, jsonFileSize, 1, pJsonFile);
        fclose(pJsonFile);

        // to do parse json

        free(jsonFileContent);

        jsonFileContent = NULL;
        pJsonFile = NULL;
        jsonFileSize = 0;
    }

    for(size_t i = 0; i < fileList.size(); i++)
    {
        free(fileList[i].fileName);
    }
    fileList.clear();

    return 0;
}

