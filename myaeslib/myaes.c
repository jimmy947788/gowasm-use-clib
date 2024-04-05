#include <stdio.h>
#include "myaes.h"

#include "aesEnc.h"
#include "aesDec.h"
#include "aesB64.h"

int Encrypt(const uint8_t* sdkSecrty, const char* plaintext, char* out)
{
    int pOutLength;
    uint8_t pOut[POUT_LEN] = {0};
    int pOutLengthB64;
    char pOutB64[POUT_B64_LEN] = {0};
    char rPlaintext[PLAINTXT_LEN] = {0};
    int rPlaintextLength;
    
    int plaintextLength = strlen(plaintext) +1;
    if (plaintextLength != PLAINTXT_LEN) {
        printf("Invalid plaintext length(%d), need %d length.\n", plaintextLength -1, PLAINTXT_LEN -1);
        return -1;
    }

    // Encrypt plaintext to pOut (bytes)
    if(sdkPkEncrypt(0, (uint8_t*) plaintext, strlen(plaintext), sdkSecrty, pOut, &pOutLength) < 0) {
        printf("Encrypt failed.\n");
        return -1;
    }

    // Encode pOut (bytes) to pOut (base64)
    if(b64_encode(pOut, pOutLength, pOutB64, &pOutLengthB64) < 0) {
        printf("Base64 encode failed.\n");
        return -1;
    }

    strcpy(out, pOutB64);
    
    return strlen(out);
}

int Decrypt(const uint8_t* sdkSecrty, const char* pOutB64, char* out)
{
    int pOutLengthB64 = strlen(pOutB64)+1;
    int pOutLength;
    uint8_t pOut[POUT_LEN] = {0};
    char rPlaintext[PLAINTXT_LEN] = {0};
    int rPlaintextLength;

    if (pOutLengthB64 != POUT_B64_LEN) {
        printf("Invalid pOutB64 length(%d), need %d length.\n", pOutLengthB64 -1, POUT_B64_LEN -1);
        return -1;
    }
    
    // Decode pOut (base64) to pOut (bytes)
    if(b64_decode(pOutB64, pOutLengthB64, pOut, &pOutLength) < 0) {
        printf("Base64 decode failed.\n");
        return -1;
    }

    // Decrypt pOut (bytes) to plaintext
    if(sdkPkDecrypt(pOut, pOutLength, sdkSecrty, (uint8_t*) rPlaintext, &rPlaintextLength) < 0) {
        printf("Decrypt failed.\n");
        return -1;
    }
    
    strcpy(out, rPlaintext);
}
