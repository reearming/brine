CC=		clang
CFLAGS=	-O2 -fno-strict-aliasing -Wall -Wextra
#CFLAGS=	-O0
CFLAGSLIB=	-target x86_64-pc-windows-msvc -shared -fuse-ld=lld -nostdlib

LIB=	/usr/local/lib/brine
BIN=	/usr/local/bin

BINSRCS=	loader/main.c loader/loader.c 
OUT=	out

all:	brine

brine: object 
	$(CC) $(CFLAGS) -o brine main.o loader.o

object: $(BINSRCS)
	$(CC) $(CFLAGS) -c $(BINSRCS)

test.dll: lib/test.c
	mkdir -p out
	$(CC) $(CFLAGSLIB) -o test.dll lib/test.c

test.exe:
	$(CC) --target=x86_64-pc-windows-gnu loader/test.asm -o loader/test.exe

clean:
	rm -f *.o .depend*

install: brine
	mkdir -p $(LIB)
	cp brine /usr/local/bin

uninstall: $(BIN)/brine $(LIB)
	rm -rf $(BIN)/brine $(LIB)
