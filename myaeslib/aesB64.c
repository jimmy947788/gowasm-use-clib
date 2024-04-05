#include "aesB64.h"

#define BASE64CHAR "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
int b64_encoded_size(int inlen) {
    int ret;

    ret = inlen;

    if (inlen % 3 != 0)
        ret += 3 - (inlen % 3);

    ret /= 3;
    ret *= 4;

    return ret;
}

int b64_encode(const unsigned char *in, int len, char *outStr, int *out_len) {
    /* char *out;*/
    int  elen, i, j, v, ret = -1;

    if (in == NULL || len == 0)
        return ret;

    elen = b64_encoded_size(len);
    out_len[0] = (int)elen;
    /* printf("%d\r\n", elen); */
    /* out = malloc(elen + 1); */
    outStr[elen] = '\0';

    for (i = 0, j = 0; i<len; i += 3, j += 4) {
         v = in[i];
         v = i + 1 < len ? v << 8 | in[i + 1] : v << 8;
         v = i + 2 < len ? v << 8 | in[i + 2] : v << 8;

         outStr[j] = BASE64CHAR[(v >> 18) & 0x3F];
         outStr[j + 1] = BASE64CHAR[(v >> 12) & 0x3F];
         if (i + 1 < len) {
             outStr[j + 2] = BASE64CHAR[(v >> 6) & 0x3F];
         } else {
             outStr[j + 2] = '=';
         } 
         if (i + 2 < len) {
             outStr[j + 3] = BASE64CHAR[v & 0x3F];
         } else {
             outStr[j + 3] = '=';
         }
    }

    ret = out_len[0];
    return ret;
}

// Simple decodeBase64 Implementation. Assume strlen(str) % 4 == 0.
int b64_decode(char *src, int len, uint8_t *dest, int *out_len) {
    int destIdx = 0;
    int  ret = -1;

    // Should be implemented
    for (int i = 0; i < len; i += 4) {
        int r = 0;
        for (int j = 0; j < 4; j ++) {
            int tmp, shift = (3 - j) * 6;
            if (src[i + j] == 43)      { tmp = 62; }
            else if (src[i + j] == 47) { tmp = 63; }
            else if (src[i + j] <= 57) { tmp = src[i + j] + 4; }
            else if (src[i + j] <= 90) { tmp = src[i + j] - 65; }
            else                       { tmp = src[i + j] - 71; }
            r += tmp << shift;
        }
        for (int j = 0; j < 3; j ++) {
            dest[destIdx ++] = r >> ((2 - j) * 8) & 0xff;
        }
    }
    *out_len = len / 4 * 3; 

    ret = out_len[0];
    return ret;
}