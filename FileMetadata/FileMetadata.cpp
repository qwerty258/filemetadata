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
    cout << "parameter error\nFileMetadata -jsonfiles files -d directories\n" << endl;
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
    vector<FilenameFilesizeSHA> fileList;
    FILE* pJsonFile = NULL;
    long jsonFileSize = 0;
    char* jsonFileContent = NULL;
    char* pEndPosition = NULL;
    json_value* pJsonValue = NULL;
    json_value* pJsonValuetemp = NULL;
    json_value* pJsonValueInloop = NULL;
    FilenameFilesizeSHA FilenameFilesizeSHATemp = {0};
    for(size_t i = 0; i < jsonfiles.size(); i++)
    {
        pJsonFile = fopen(jsonfiles[i], "rb");
        fseek(pJsonFile, 0, SEEK_END);
        jsonFileSize = ftell(pJsonFile);
        rewind(pJsonFile);
        jsonFileContent = (char*)malloc(jsonFileSize);
        fread(jsonFileContent, jsonFileSize, 1, pJsonFile);
        fclose(pJsonFile);
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

