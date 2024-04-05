package mycrypto

/*
#cgo LDFLAGS: -L. -lmyaes
#include "myaes.h"
*/
import "C"
import (
	"unsafe"
)

// Encrypt 调用C库中的Encrypt函数
func AesEncrypt(secrtyKey []byte, plaintext string) string {
	out := make([]byte, 256) // 假设输出不会超过256字节
	C.Encrypt((*C.uint8_t)(&secrtyKey[0]), C.CString(plaintext), (*C.char)(unsafe.Pointer(&out[0])))

	//strip ending empty string
	for i := 0; i < len(out); i++ {
		if out[i] == 0 {
			out = out[:i]
			break
		}
	}
	return string(out)
}

// Decrypt 调用C库中的Decrypt函数
func AesDecrypt(secrtyKey []byte, ciphertext string) string {
	out := make([]byte, 256) // 假设输出不会超过256字节
	C.Decrypt((*C.uint8_t)(&secrtyKey[0]), C.CString(ciphertext), (*C.char)(unsafe.Pointer(&out[0])))

	//strip ending empty string
	for i := 0; i < len(out); i++ {
		if out[i] == 0 {
			out = out[:i]
			break
		}
	}
	return string(out)
}
