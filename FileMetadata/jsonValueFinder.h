#pragma once

#ifdef _cplusplus
extern "C" {
#endif // _cplusplus

    void* jsonObjectFinder(void* pJsonValue, char* objectName);

    char* jsonStringDup(void* pJsonValue);

#ifdef _cplusplus
}
#endif // _cplusplus