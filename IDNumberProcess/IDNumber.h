#ifndef _ID_NUMBER_H_
#define _ID_NUMBER_H_

#define IDN_RESULT_OK       0
#define IDN_WRONG_LEN       -1
#define IDN_NOT_DIGIT       -2
#define IDN_INVALID_HANDLE  -3
#define IDN_PROCESSED       -4
#define IDN_INVALID_ID      -5
#define IDN_NOT_PROCESSED   -6

typedef void* ID_number_handle;

ID_number_handle IDN_get_handle(char* ID_string);

int IDN_process_ID(ID_number_handle handle);

int IDN_get_area_code(ID_number_handle handle, char** string);

int IDN_get_birthdate(ID_number_handle handle, char** string);

int IDN_get_order_number(ID_number_handle handle, char** string);

int IDN_get_check(ID_number_handle handle, char** string);

int IDN_reset_handle(ID_number_handle handle, char* ID_string);

void IDN_free_handle(ID_number_handle handle);

#endif // !_ID_NUMBER_H_