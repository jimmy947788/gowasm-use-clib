# example-gowasm-uselib
tinygo wasm use c library (AES cbc mode)

This project demonstrates the integration of a C library with Go-based WebAssembly and a Go HTTP server. The C library is used for AES encryption, which is compiled to WebAssembly using Emscripten. The Go WebAssembly module interacts with this AES library, and both are served over an HTTP server written in Go.

## Structure

The project is divided into three main directories:

1. `myaeslib`: A C language-based AES library. It is compiled using the Emscripten compiler and includes Makefile for building and installing the library.
2. `mywasm`: Contains Go code for the WebAssembly module that utilizes the myaeslib for encryption. It is compiled using the TinyGo compiler and includes Makefile for building and installing.
3. `goserver`: A simple HTTP server written in Go that serves the WebAssembly module and provides an interface to interact with the AES encryption functionality.

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

Run Makefile commands to build and install the AES library:

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

## Usage

Once the server is running, navigate to http://localhost:8080 in your web browser. You should see an interface that allows you to interact with the AES encryption functionality provided by the myaeslib library through WebAssembly.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues to improve the project.

## License

This project is licensed under the MIT [License](./LICENSE) - see the LICENSE file for details.