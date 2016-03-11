#include <stdio.h>
#include <stdlib.h>
#include "IDNumber.h"
#include "ID2MySQL.h"

void get_rid_of(char* string)
{
    while('\0' != *string)
    {
        if('\r' == *string || '\n' == *string)
        {
            *string = '\0';
        }
        string++;
    }
}

void spilt_buffer(char* buffer, char* name, char* ID)
{
    size_t i;
    size_t j;
    for(i = 0, j = 0; i < 1024; i++, j++)
    {
        if(',' != buffer[i])
        {
            name[j] = buffer[i];
        }
        else
        {
            break;
        }
    }
    i++;
    for(j = 0; i < 1024; i++, j++)
    {
        if('\0' != buffer[i])
        {
            ID[j] = buffer[i];
        }
        else
        {
            break;
        }
    }

    get_rid_of(name);
    get_rid_of(ID);
}

int main(int argc, char* argv[])
{
    FILE* p_file = fopen("D:\\Names and ID.csv", "r");
    char* buffer = calloc(1, 1024);
    char* name = calloc(1, 1024);
    char* ID = calloc(1, 1024);
    ID_number_handle handle;
    int result;
    char* area_code;
    char* birth_date;
    char* order_number;
    char* check_number;
    handle = IDN_get_handle("");
    initial_mysql_connector();
    connect_mysql_connector("localhost", "root", "123456", "ID_names", 3306);
    if(NULL != buffer)
    {
        while(NULL != fgets(buffer, 1024, p_file))
        {
            spilt_buffer(buffer, name, ID);
            IDN_reset_handle(handle, ID);
            result = IDN_process_ID(handle);
            if(0 != result)
            {
                printf("ID INVALID\n");
            }
            else
            {
                IDN_get_area_code(handle, &area_code);
                IDN_get_birthdate(handle, &birth_date);
                IDN_get_order_number(handle, &order_number);
                IDN_get_check(handle, &check_number);
                if(0 == find_an_ID_mysql_connector(
                    area_code,
                    birth_date,
                    order_number,
                    check_number))
                {
                    insert_mysql_connector(area_code, birth_date, order_number, check_number);
                }
                else
                {
                    printf("ALREADY IN DATABASE\n");
                }
            }
        }
    }
    uninitial_mysql_connector();
    IDN_free_handle(handle);
    free(buffer);
    free(name);
    free(ID);
    fclose(p_file);
    return 0;
}

