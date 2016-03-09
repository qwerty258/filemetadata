#include <stdio.h>
#include "IDNumber.h"

int main(int argc, char* argv[])
{
    char* IDs[] = {
        "330183199008250017",
        "daiiiodaghjkghhhhh",
        "33018319900825001X",
        "dsafdak",
        "51140019861016781x",
        "5114dun9861016781x"
    };

    char* temp = NULL;
    int result = IDN_RESULT_OK;
    size_t total_tests = sizeof(IDs) / sizeof(char*);
    ID_number_handle handle = IDN_get_handle("");

    for(size_t i = 0; i < total_tests; i++)
    {
        printf("Test: %s\n", IDs[i]);

        IDN_reset_handle(handle, IDs[i]);

        result = IDN_process_ID(handle);
        switch(result)
        {
            case IDN_RESULT_OK:
                IDN_get_area_code(handle, &temp);
                printf("area code: %s\n", temp);

                IDN_get_birthdate(handle, &temp);
                printf("birth date: %s\n", temp);

                IDN_get_order_number(handle, &temp);
                printf("order num: %s\n", temp);

                IDN_get_check(handle, &temp);
                printf("check: %s\n", temp);
                break;
            case IDN_WRONG_LEN:
                printf("IDN_WRONG_LEN\n");
                break;
            case IDN_NOT_DIGIT:
                printf("IDN_NOT_DIGIT\n");
                break;
            case IDN_INVALID_ID:
                printf("IDN_INVALID_ID\n");
                break;
            default:
                break;
        }

        printf("\n\n");
    }

    IDN_free_handle(handle);

    return 0;
}

