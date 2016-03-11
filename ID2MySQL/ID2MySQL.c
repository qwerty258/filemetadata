#include "ID2MySQL.h"
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _mysql_context
{
    MYSQL* p_mysql;
}mysql_context;

mysql_context* global_context;

int initial_mysql_connector(void)
{
    global_context = calloc(1, sizeof(mysql_context));
    if(NULL != global_context)
    {
        global_context->p_mysql = mysql_init(NULL);
        if(NULL == global_context->p_mysql)
        {
            free(global_context);
            return -1;
        }
    }
    else
    {
        return -1;
    }
    return 0;
}

int connect_mysql_connector(char* host, char* username, char* password, char* db, unsigned int port)
{
    if(NULL == mysql_real_connect(global_context->p_mysql, host, username, password, db, port, NULL, 0))
    {
        return -1;
    }
    return 0;
}

int find_an_ID_mysql_connector(char* area_code, char* birth_date, char* order_number, char* check_number)
{
    char* sql = calloc(1, 1024);
    sprintf(
        sql,
        "SELECT * FROM test WHERE area_code=%s AND birth_date='%s' AND order_number=%s AND check_number='%s';",
        area_code,
        birth_date,
        order_number,
        check_number);
    mysql_query(global_context->p_mysql, sql);

    MYSQL_RES* p_mysql_result = mysql_use_result(global_context->p_mysql);

    int result = 0;

    MYSQL_ROW mysql_row = mysql_fetch_row(p_mysql_result);
    if(NULL == mysql_row)
    {
        result = 0;
    }
    else
    {
        result = 1;
    }

    mysql_free_result(p_mysql_result);
    free(sql);
    return result;
}

int insert_mysql_connector(char* area_code, char* birth_date, char* order_number, char* check_number)
{
    char* sql = calloc(1, 1024);
    sprintf(
        sql,
        "INSERT INTO test VALUES(%s,'%s',%s,'%s');",
        area_code,
        birth_date,
        order_number,
        check_number);
    mysql_query(global_context->p_mysql, sql);

    mysql_free_result(mysql_use_result(global_context->p_mysql));

    free(sql);
    return 0;
}

int uninitial_mysql_connector(void)
{
    mysql_close(global_context->p_mysql);
    free(global_context);
    return 0;
}