CC=emcc
AR=emar
RANLIB=emranlib
SRCS=aesB64.c aesEnc.c aesDec.c myaes.c
OBJS=$(SRCS:.c=.o)
STATIC_LIB=libmyaes.a
DYNAMIC_LIB=libmyaes.so
OUTDIR ?= ../output
INSTALLDIR ?= ../mywasm/mycrypto

.PHONY: all static dynamic clean install

all: static #dynamic

static: $(STATIC_LIB)

$(STATIC_LIB): $(OBJS)
	$(AR) rc $(STATIC_LIB) $(OBJS)
	$(RANLIB) $(STATIC_LIB)
	mkdir -p $(OUTDIR)
	cp -f myaes.h $(OUTDIR)
	mv -f $(STATIC_LIB) $(OUTDIR)
	rm -f $(OBJS)

dynamic: $(DYNAMIC_LIB)

$(DYNAMIC_LIB): $(OBJS)
	$(CC) -shared -o $(DYNAMIC_LIB) $(OBJS)
	mkdir -p $(OUTDIR)
	cp -f myaes.h $(OUTDIR)
	mv -f $(DYNAMIC_LIB) $(OUTDIR)
	rm -f $(OBJS)

%.o: %.c
	$(CC) -fPIC -c $< -o $@ $(CFALGS)

clean:
	rm -f $(OBJS) $(OUTDIR)/$(STATIC_LIB) $(OUTDIR)//$(DYNAMIC_LIB)

install:
	cp -f myaes.h $(INSTALLDIR)
	cp -f $(OUTDIR)/$(STATIC_LIB) $(INSTALLDIR)
#cp -f $(OUTDIR)/$(DYNAMIC_LIB) $(INSTALLDIR)
	
