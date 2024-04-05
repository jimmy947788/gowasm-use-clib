#ifndef AES_ENC_H
#define AES_ENC_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

int16_t sdkPkEncrypt(int mode, uint8_t* in, int in_length, const uint8_t* key, uint8_t* out, int* out_length);

#ifdef __cplusplus
        }
#endif
#endif
