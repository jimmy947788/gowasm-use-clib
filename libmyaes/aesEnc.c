
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
// #include <stdio.h>
#include <string.h>
#include "aesEnc.h"

// #if defined(AES256) && (AES256 == 1)
    // #define NUM_ROUNDS 14
    // #define KEY_SIZE 32 // Bytes
// #elif defined(AES192) && (AES192 == 1)
    // #define NUM_ROUNDS 12
    // #define KEY_SIZE 24 // Bytes
// #else
    #define NUM_ROUNDS 10
    #define KEY_SIZE 16 // Bytes
// #endif
//

static const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static const uint8_t rcon[11] = {0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

static uint8_t xtime(uint8_t x) {
    uint8_t a = (((x>>7) & 1) * 0x1b);
    uint8_t b = (uint8_t)(((int)x)<<1);
    return a ^ b;
}

static uint8_t multiply(uint8_t x, uint8_t y) {
    return (((y & 1) * x) ^
            ((y>>1 & 1) * xtime(x)) ^
            ((y>>2 & 1) * xtime(xtime(x))) ^
            ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^
            ((y>>4 & 1) * xtime(xtime(xtime(xtime(x)))))); 
}

static void subBytes(uint8_t* state) {
    for (int i = 0; i < 16; i ++) {
        state[i] = sbox[state[i]];
    }
}

static void mixColumns(uint8_t* state) {
    for (int i = 0; i < 4; i ++) {
        uint8_t tmp[4];
        tmp[0] = multiply(state[i * 4 + 0], 2) ^ multiply(state[i * 4 + 1], 3) ^ multiply(state[i * 4 + 2], 1) ^ multiply(state[i * 4 + 3], 1);
        tmp[1] = multiply(state[i * 4 + 0], 1) ^ multiply(state[i * 4 + 1], 2) ^ multiply(state[i * 4 + 2], 3) ^ multiply(state[i * 4 + 3], 1);
        tmp[2] = multiply(state[i * 4 + 0], 1) ^ multiply(state[i * 4 + 1], 1) ^ multiply(state[i * 4 + 2], 2) ^ multiply(state[i * 4 + 3], 3);
        tmp[3] = multiply(state[i * 4 + 0], 3) ^ multiply(state[i * 4 + 1], 1) ^ multiply(state[i * 4 + 2], 1) ^ multiply(state[i * 4 + 3], 2);
        memcpy(state + i * 4, tmp, 4);
    }
}

static void shiftRows(uint8_t* state) {
    uint8_t temp;

    // Second Row
    temp = state[1];
    state[1 + 0] = state[1 + 4];
    state[1 + 4] = state[1 + 8];
    state[1 + 8] = state[1 + 12];
    state[1 + 12] = temp;

    // Third Row
    temp = state[2 + 0];
    state[2 + 0] = state[2 + 8];
    state[2 + 8] = temp;
    temp = state[2 + 4];
    state[2 + 4] = state[2 + 12];
    state[2 + 12] = temp;

    // Last Row
    temp = state[3 + 12];
    state[3 + 12] = state[3 + 8];
    state[3 + 8] = state[3 + 4];
    state[3 + 4] = state[3 + 0];
    state[3 + 0] = temp;
}

static void addRoundKey(uint8_t round, uint8_t* state, const uint8_t* keys) {
    for (int i = 0; i < 16; i ++) {
        state[i] ^= keys[round * 16 + i];
    }
}

static void expandKeys(uint8_t roundKeys[], const uint8_t *key) {
    uint8_t tmpKey[4];
    int nk = KEY_SIZE / 4;
    memcpy(roundKeys, key, KEY_SIZE);

    for (int i = nk; i < 4 * (NUM_ROUNDS + 1); i ++) {
        memcpy(tmpKey, roundKeys + (i - 1) * 4, 4);
        if (i % nk == 0) {
            uint8_t tmp;
            // Rotword
            tmp = tmpKey[0];
            tmpKey[0] = tmpKey[1];
            tmpKey[1] = tmpKey[2];
            tmpKey[2] = tmpKey[3];
            tmpKey[3] = tmp;

            // Subword
            for (int j = 0; j < 4; j ++) tmpKey[j] = sbox[tmpKey[j]];

            // rcon
            tmpKey[0] ^= rcon[i / nk];
        }


        if (nk > 6 && i % nk == 4) {
            //Subword
            for (int j = 0; j < 4; j ++) tmpKey[j] = sbox[tmpKey[j]];
        }

        for (int j = 0; j < 4; j ++) {
            roundKeys[i * 4 + j] = roundKeys[(i - nk) * 4 + j] ^ tmpKey[j];
        }
    }
}

static void aesEncBlock(uint8_t *roundKeys, uint8_t *data) {
    addRoundKey(0, data, roundKeys);
    for (int i = 1; i < NUM_ROUNDS + 1; i ++) {
        subBytes(data);
        shiftRows(data);
        if (i != NUM_ROUNDS) {
            mixColumns(data);
        }
        addRoundKey(i, data, roundKeys);
    }
}

static void xorArray(uint8_t arr1[], const uint8_t *arr2, int length) {
    for (int i = 0; i < length; i ++) {
        arr1[i] ^= arr2[i];
    }
}

static void aesEncCBC(uint8_t* in, int inLength, const uint8_t* key, const uint8_t* iv, uint8_t* out, int* outLength) {
    uint8_t roundKeys[(NUM_ROUNDS + 1) * KEY_SIZE];
    const uint8_t *orgIv = iv, *orgKey = key;

    *outLength = (inLength / 16 + (inLength % 16 != 0)) * 16;
    memset(out, 0, *outLength);
    memcpy(out, in, inLength);

    expandKeys(roundKeys, iv);
    iv = orgKey;
    for (int i = 0; i < *outLength / 16; i ++) {
        xorArray(out + i * 16, iv, 16);
        aesEncBlock(roundKeys, out + i * 16);
        iv = out + i * 16;
    }

    key = orgKey;
    expandKeys(roundKeys, key);
    iv = orgIv;
    for (int i = *outLength / 16 - 1; i >= 0; i --) {
        xorArray(out + i * 16, iv, 16);
        aesEncBlock(roundKeys, out + i * 16);
        iv = out + i * 16;
    }
}

int16_t sdkPkEncrypt(int mode, uint8_t* in, int inLength, const uint8_t* key, uint8_t* out, int* outLength) {
    aesEncCBC(in, inLength, key + 16, key, out, outLength);
    return outLength[0];
}
#ifdef __cplusplus
}
#endif
