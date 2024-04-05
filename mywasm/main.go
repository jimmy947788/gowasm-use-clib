package main

import (
	"log"
	"mywasm/mycrypto"
	"syscall/js"
)

var VERSION string
var secrtyKey = []byte{}

func AesInit(this js.Value, args []js.Value) interface{} {
	// pass byte array from js to go
	secrtyKey = make([]byte, args[0].Length())
	js.CopyBytesToGo(secrtyKey, args[0])

	log.Println("secrtyKey:", secrtyKey)

	return nil
}

func AesEncrypt(this js.Value, args []js.Value) interface{} {
	message := args[0].String()
	ciphertext := mycrypto.AesEncrypt(secrtyKey, message)
	log.Println("ciphertext:", ciphertext)
	return js.ValueOf(ciphertext)
}

func AesDecrypt(this js.Value, args []js.Value) interface{} {
	message := args[0].String()
	plaintext := mycrypto.AesDecrypt(secrtyKey, message)
	log.Println("plaintext:", plaintext)
	return js.ValueOf(plaintext)
}

func main() {
	// Instantiate a channel, 'ch', with no buffer, acting as a synchronization point for the goroutine.
	ch := make(chan struct{}, 0)

	println("adding two numbers:", add(2, 3)) // expecting 5

	js.Global().Set("AesInit", js.FuncOf(AesInit))
	js.Global().Set("AesEncrypt", js.FuncOf(AesEncrypt))
	js.Global().Set("AesDecrypt", js.FuncOf(AesDecrypt))

	log.Println("WASM Go Initialized, version:", VERSION)

	//gomore.API_DOMAIN = "https://saas-api.qa.gomore.club"
	// Utilize a channel receive expression to halt the 'main' goroutine, preventing the program from terminating.
	<-ch
}

// This function is imported from JavaScript, as it doesn't define a body.
// You should define a function named 'add' in the WebAssembly 'env'
// module from JavaScript.
//
//export add
func add(x, y int) int

// This function is exported to JavaScript, so can be called using
// exports.multiply() in JavaScript.
//
//export multiply
func multiply(x, y int) int {
	return x * y
}
