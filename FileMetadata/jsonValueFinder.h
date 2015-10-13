#pragma once
#include "json.h"

#ifdef _cplusplus
extern "C" {
#endif // _cplusplus

    json_value* jsonObjectFinder(json_value* pJsonValue, char* objectName);

    char* jsonStringDup(json_value* pJsonValue);

#ifdef _cplusplus
}
#endif // _cplusplus