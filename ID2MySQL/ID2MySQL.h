#ifndef _ID_TO_MYSQL_H_
#define _ID_TO_MYSQL_H_

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

    int initial_mysql_connector(void);

    int connect_mysql_connector(char* host, char* username, char* password, char* db, unsigned int port);

    int find_an_ID_mysql_connector(char* area_code, char* birth_date, char* order_number, char* check_number);

    int insert_mysql_connector(char* area_code, char* birth_date, char* order_number, char* check_number);

    int uninitial_mysql_connector(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !_ID_TO_MYSQL_H_