// aesB64.h
#ifndef AESB64_H
#define AESB64_H


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

int b64_encoded_size(int inlen);

int b64_encode(const unsigned char *in, int len, char *outStr, int *out_len);

int b64_decode(char *src, int len, uint8_t *dest, int *out_len);

#endif