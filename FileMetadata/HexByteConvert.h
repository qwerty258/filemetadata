#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    void Hex2Byte(const char* hexString, unsigned char* byteArray, size_t byteArraySize);

    char* Byte2Hex(unsigned char* byteArray, size_t byteArraySize);

#ifdef __cplusplus
}
#endif // __cplusplus