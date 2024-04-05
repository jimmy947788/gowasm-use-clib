// myaes.h
#ifndef MYAES_H
#define MYAES_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <time.h>


#define SECRET_KEY_SIZE 32
#define POUT_LEN 48 + 1
#define POUT_B64_LEN 64 + 1
#define PLAINTXT_LEN 48 + 1


int Encrypt(const uint8_t* sdkSecrty, const char* plaintext, char* out);
int Decrypt(const uint8_t* sdkSecrty, const char* pOutB64, char* out);

#endif