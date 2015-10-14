#include "jsonValueFinder.h"

void* jsonObjectFinder(void* pJsonValue, char* objectName)
{
    void* pJsonValueTemp = 0;
    //if(NULL == pJsonValue || NULL == objectName || json_object != pJsonValue->type)
    //{
    //    return pJsonValueTemp;
    //}
    //for(unsigned int i = 0; i < pJsonValue->u.object.length; i++)
    //{
    //    if(0 == memcmp(objectName, pJsonValue->u.object.values[i].name, pJsonValue->u.object.values[i].name_length))
    //    {
    //        pJsonValueTemp = pJsonValue->u.object.values[i].value;
    //        return pJsonValueTemp;
    //    }
    //}
    return pJsonValueTemp;
}

char* jsonStringDup(void* pJsonValue)
{
    char* strTemp = 0;
    //if(NULL == pJsonValue || json_string != pJsonValue->type)
    //{
    //    return strTemp;
    //}
    //strTemp = (char*)calloc(1, pJsonValue->u.string.length + 5);
    //if(NULL != strTemp)
    //{
    //    memcpy(strTemp, pJsonValue->u.string.ptr, pJsonValue->u.string.length);
    //}
    return strTemp;
}