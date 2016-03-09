#include "IDNumber.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define ID_LEN              18
#define AREA_CODE_LEN       6
#define YEAR_LEN            4
#define MONTH_LEN           2
#define DAY_LEN             MONTH_LEN
#define ORDER_NUMBER_LEN    3

int32_t first[ID_LEN - 1] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
char second[11] = {'1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2'};

typedef struct _IDN_context
{
    bool ID_valid;
    bool ID_processed;
    char* p_area_code;
    char* p_birth_year;
    char* p_birth_month;
    char* p_birth_day;
    char* p_order_number;
    char* p_check;
    char str_raw_string[ID_LEN + 6];
    char str_area_code[AREA_CODE_LEN + 2];
    char str_birth_date[YEAR_LEN + 1 + MONTH_LEN + 1 + DAY_LEN + 2];
    char str_three_number[ORDER_NUMBER_LEN + 5];
}IDN_context;

ID_number_handle IDN_get_handle(char* ID_string)
{
    IDN_context* p_context = calloc(1, sizeof(IDN_context));
    if(NULL != p_context)
    {
        strncpy(p_context->str_raw_string, ID_string, ID_LEN + 6);
        p_context->str_raw_string[ID_LEN + 6 - 1] = '\0';
        p_context->ID_processed = false;
        p_context->ID_valid = false;
    }
    return p_context;
}

int IDN_process_ID(ID_number_handle handle)
{
    IDN_context* p_context = handle;
    uint32_t i;
    uint32_t sum = 0;

    if(NULL == p_context)
    {
        return IDN_INVALID_HANDLE;
    }

    if(p_context->ID_processed)
    {
        return IDN_PROCESSED;
    }

    if(ID_LEN != strlen(p_context->str_raw_string))
    {
        return IDN_WRONG_LEN;
    }

    for(i = 0; i < ID_LEN - 1; i++)
    {
        if(!isdigit(p_context->str_raw_string[i]))
        {
            break;
        }
    }

    if(ID_LEN - 1 != i)
    {
        return IDN_NOT_DIGIT;
    }

    for(i = 0; i < ID_LEN - 1; i++)
    {
        sum += (p_context->str_raw_string[i] - '0') * first[i];
    }
    sum %= 11;
    if('X' == second[sum])
    {
        if('X' != toupper(p_context->str_raw_string[ID_LEN - 1]))
        {
            return IDN_INVALID_ID;
        }
        else
        {
            p_context->str_raw_string[ID_LEN - 1] = toupper(p_context->str_raw_string[ID_LEN - 1]);
        }
    }
    else
    {
        if(second[sum] != p_context->str_raw_string[ID_LEN - 1])
        {
            return IDN_INVALID_ID;
        }
    }

    p_context->ID_valid = true;

    p_context->p_area_code = p_context->str_raw_string;
    p_context->p_birth_year = p_context->p_area_code + AREA_CODE_LEN;
    p_context->p_birth_month = p_context->p_birth_year + YEAR_LEN;
    p_context->p_birth_day = p_context->p_birth_month + MONTH_LEN;
    p_context->p_order_number = p_context->p_birth_day + DAY_LEN;
    p_context->p_check = p_context->p_order_number + ORDER_NUMBER_LEN;

    memcpy(p_context->str_area_code, p_context->p_area_code, AREA_CODE_LEN);
    memcpy(p_context->str_birth_date, p_context->p_birth_year, YEAR_LEN);
    memcpy(p_context->str_birth_date + YEAR_LEN, "-", 1);
    memcpy(p_context->str_birth_date + YEAR_LEN + 1, p_context->p_birth_month, MONTH_LEN);
    memcpy(p_context->str_birth_date + YEAR_LEN + 1 + MONTH_LEN, "-", 1);
    memcpy(p_context->str_birth_date + YEAR_LEN + 1 + MONTH_LEN + 1, p_context->p_birth_day, DAY_LEN);
    memcpy(p_context->str_three_number, p_context->p_order_number, ORDER_NUMBER_LEN);

    p_context->ID_processed = true;

    return IDN_RESULT_OK;
}

int IDN_get_area_code(ID_number_handle handle, char** string)
{
    IDN_context* p_context = handle;
    if(NULL == p_context)
    {
        return IDN_INVALID_HANDLE;
    }
    if(!p_context->ID_processed)
    {
        return IDN_NOT_PROCESSED;
    }
    if(!p_context->ID_valid)
    {
        return IDN_INVALID_ID;
    }

    *string = p_context->str_area_code;

    return IDN_RESULT_OK;
}

int IDN_get_birthdate(ID_number_handle handle, char** string)
{
    IDN_context* p_context = handle;
    if(NULL == p_context)
    {
        return IDN_INVALID_HANDLE;
    }
    if(!p_context->ID_processed)
    {
        return IDN_NOT_PROCESSED;
    }
    if(!p_context->ID_valid)
    {
        return IDN_INVALID_ID;
    }

    *string = p_context->str_birth_date;

    return IDN_RESULT_OK;
}

int IDN_get_order_number(ID_number_handle handle, char** string)
{
    IDN_context* p_context = handle;
    if(NULL == p_context)
    {
        return IDN_INVALID_HANDLE;
    }
    if(!p_context->ID_processed)
    {
        return IDN_NOT_PROCESSED;
    }
    if(!p_context->ID_valid)
    {
        return IDN_INVALID_ID;
    }

    *string = p_context->str_three_number;

    return IDN_RESULT_OK;
}

int IDN_get_check(ID_number_handle handle, char** string)
{
    IDN_context* p_context = handle;
    if(NULL == p_context)
    {
        return IDN_INVALID_HANDLE;
    }
    if(!p_context->ID_processed)
    {
        return IDN_NOT_PROCESSED;
    }
    if(!p_context->ID_valid)
    {
        return IDN_INVALID_ID;
    }

    *string = p_context->p_check;

    return IDN_RESULT_OK;
}

int IDN_reset_handle(ID_number_handle handle, char* ID_string)
{
    IDN_context* p_context = handle;
    if(NULL == p_context)
    {
        return IDN_INVALID_HANDLE;
    }

    memset(p_context, 0x00, sizeof(IDN_context));

    strncpy(p_context->str_raw_string, ID_string, ID_LEN + 6);
    p_context->str_raw_string[ID_LEN + 6 - 1] = '\0';
    p_context->ID_processed = false;
    p_context->ID_valid = false;

    return IDN_RESULT_OK;
}

void IDN_free_handle(ID_number_handle handle)
{
    free(handle);
}