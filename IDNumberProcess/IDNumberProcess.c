#include <stdio.h>
#include "IDNumber.h"

#define test ID2

int main(int argc, char* argv[])
{
    char* ID1 = "330183199008250017";
    char* ID2 = "daiiioda";
    char* temp = NULL;
    int reuslt = IDN_RESULT_OK;

    ID_number_handle handle = IDN_get_handle(test);

    IDN_process_ID(handle);

    IDN_get_area_code(handle, &temp);
    printf("area code: %s\n", temp);

    IDN_get_birthdate(handle, &temp);
    printf("birth date: %s\n", temp);

    IDN_get_three_NUM(handle, &temp);
    printf("three num: %s\n", temp);

    IDN_get_check(handle, &temp);
    printf("check: %s\n", temp);

    IDN_free_handle(handle);

    return 0;
}

