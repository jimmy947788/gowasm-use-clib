# Makefile

# 变量定义
OUTDIR ?= ../output
WASM_NAME ?= mywasm
BUILD_OPT = -opt=2 -panic=trap  -no-debug
VERSION ?= 1.0.0

# 默认目标
.PHONY: all
all: wasm

# 编译WebAssembly模块
.PHONY: wasm
wasm:
	mkdir -p $(OUTDIR)
	tinygo build -o $(OUTDIR)/$(WASM_NAME).wasm \
		-target wasm \
		-ldflags="-X 'main.VERSION=$(VERSION)'"

# 清理构建文件
.PHONY: clean
clean:
	rm -rf $(OUTDIR)/$(WASM_NAME).wasm


# if  $(OUTDIR)/wasm_exec.js not exist, download from https://raw.githubusercontent.com/tinygo-org/tinygo/release/targets/wasm_exec.js 
wasm_exec:
	@if [ ! -f "$(OUTDIR)/wasm_exec.js" ]; then \
		wget https://raw.githubusercontent.com/tinygo-org/tinygo/release/targets/wasm_exec.js -O $(OUTDIR)/wasm_exec.js; \
	fi	

install: wasm_exec
	cp -f $(OUTDIR)/$(WASM_NAME).wasm ../server/static/
	cp -f $(OUTDIR)/wasm_exec.js ../server/static/