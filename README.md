# example-gowasm-uselib


tinygo wasm use c library (AES[^1] CBC[^2] mode)

This project demonstrates the integration of a C library with Go-based WebAssembly and a Go HTTP server. The C library is used for AES[^1] encryption, which is compiled to WebAssembly using Emscripten. The Go WebAssembly module interacts with this AES[^1] library, and both are served over an HTTP server written in Go.

## Structure

The project is divided into three main directories:

1. `myaeslib`: A C language-based AES[^1] library. It is compiled using the Emscripten compiler and includes Makefile for building and installing the library.
2. `mywasm`: Contains Go code for the WebAssembly module that utilizes the myaeslib for encryption. It is compiled using the TinyGo compiler and includes Makefile for building and installing.
3. `goserver`: A simple HTTP server written in Go that serves the WebAssembly module and provides an interface to interact with the AES[^1] encryption functionality.

## Getting Started

### Prerequisites

- Install [Emscripten](https://emscripten.org/docs/getting_started/downloads.html) for compiling C code to WebAssembly.
- Install [TinyGo](https://tinygo.org/getting-started/install/) for compiling Go code to WebAssembly.
- Install [Go](https://golang.org/doc/install) for running the HTTP server.

### Building and Installing myaeslib

Navigate to the `myaeslib` directory:

```bash
cd myaeslib
```

Run Makefile commands to build and install the AES[^1] library:

```bash
make
make install
```
> install: copy `libmyaes.a` and `myaes.h`from [output](./output/) directory to [mywasm/mycrypto](./mywasm/mycrypto)

### Building and Installing mywasm

Navigate to the `mywasm` directory:

```bash
cd mywasm
```

Run Makefile commands to build and install the WebAssembly module:

```bash
make
make install
```
> install: copy `mywasm.wasm` and `wasm_exec.js`from [output](./output/) directory to [server/static](./server/static)


### Running the HTTP Server

Navigate to the `goserver` directory:

```bash
cd goserver
```

Run the HTTP server:

```bash
go run main.go
```

The server will start, and you can access the WebAssembly module through the web browser at http://localhost:8080.

## Usage Instructions

Starting the Server
Once the server is running, navigate to `http://localhost:8080` in your web browser. You should see an interface that allows you to interact with the AES[^1] encryption functionality provided by the `myaeslib` library through WebAssembly.

Linking the Static Library
The C library functions are defined in [myaes.h](mywasm/mycrypto/myaes.h) as follows:

- `int Encrypt(const uint8_t* sdkSecrty, const char* plaintext, char* out);`: Encryption function
- `int Decrypt(const uint8_t* sdkSecrty, const char* pOutB64, char* out);`: Decryption function

To use these functions in Go code, prepare a Go package [myaes.go](mywasm/mycrypto/myaes.go) to link the static library `libmyaes.a`.

### Go Code Example
To call `libmyaes.a` using cgo, here's how you can link the static library and call the encryption and decryption functions in your Go code:

```go
/*
#cgo LDFLAGS: -L. -lmyaes
#include "myaes.h"
*/
import "C"
import (
    "unsafe"
)

// Assume secrtyKey is a []byte slice containing the security key,
// plaintext is the string to encrypt, and ciphertext is the encrypted string.
// out is used to store the result of encryption or decryption.

// Encryption example
func Encrypt(secrtyKey []byte, plaintext string) string {
    var out [128]byte // Assume the output will not exceed 128 bytes
    C.Encrypt((*C.uint8_t)(&secrtyKey[0]), C.CString(plaintext), (*C.char)(unsafe.Pointer(&out[0])))
    return string(out[:])
}

// Decryption example
func Decrypt(secrtyKey []byte, ciphertext string) string {
    var out [128]byte // Assume the output will not exceed 128 bytes
    C.Decrypt((*C.uint8_t)(&secrtyKey[0]), C.CString(ciphertext), (*C.char)(unsafe.Pointer(&out[0])))
    return string(out[:])
}
```

**Note:** These examples assume the output will not exceed 128 bytes. In practice, depending on the size of your encrypted text, you might need to adjust the size of the `out` array.

Furthermore, the examples do not handle memory allocation and release for the strings returned from `C.CString` and `C.Encrypt`/`C.Decrypt` calls. In actual applications, ensure proper memory management to avoid memory leaks.


[^1]: Advanced Encryption Standard
[^2]: Cipher Block Chaining

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues to improve the project.

## License

This project is licensed under the MIT [License](./LICENSE) - see the LICENSE file for details.
