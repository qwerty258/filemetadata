#include "json.h"
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
    json_value* pJsonValue = NULL;
    json_value* pJsonValuetemp = NULL;
    json_object_entry* pJsonObjectEntryTemp = NULL;
    for(size_t i = 0; i < jsonfiles.size(); i++)
    {
        pJsonFile = fopen(jsonfiles[i], "rb");
        fseek(pJsonFile, 0, SEEK_END);
        jsonFileSize = ftell(pJsonFile);
        rewind(pJsonFile);
        jsonFileContent = (char*)malloc(jsonFileSize);
        fread(jsonFileContent, jsonFileSize, 1, pJsonFile);
        pJsonValue = json_parse(jsonFileContent, jsonFileSize);

        if(json_object == pJsonValue->type)
        {
            if(NULL != pJsonValue->u.object.values)
            {
                pJsonObjectEntryTemp = pJsonValue->u.object.values;
                if(0 == memcmp("data", pJsonObjectEntryTemp->name, pJsonObjectEntryTemp->name_length))
                {
                    if(NULL != pJsonObjectEntryTemp->value)
                    {
                        pJsonValuetemp = pJsonObjectEntryTemp->value;
                        if(json_array == pJsonValuetemp->type)
                        {
                            fileList.resize(pJsonValuetemp->u.array.length);
                            for(unsigned int i = 0; i < pJsonValuetemp->u.array.length; i++)
                            {
                                //  (*pJsonValuetemp->u.array.values[i]).
                            }
                        }
                    }
                }
            }
        }

        json_value_free(pJsonValue);
        free(jsonFileContent);
        fclose(pJsonFile);

        pJsonValue = NULL;
        pJsonValuetemp = NULL;
        jsonFileContent = NULL;
        pJsonFile = NULL;
        jsonFileSize = 0;
    }
    return 0;
}

