#ifndef AES_DEC_H
#define AES_DEC_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

int16_t sdkPkDecrypt(uint8_t* in, int in_length, const uint8_t* key, uint8_t* out, int* out_length);

#ifdef __cplusplus
        }
#endif
#endif
