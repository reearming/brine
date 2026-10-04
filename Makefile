CC=		clang
CFLAGS=	-O2 -fno-strict-aliasing -Wall -Wextra
#CFLAGS=	-O0

LIB=	/usr/local/lib/brine
BIN=	/usr/local/bin

SRCS=	loader/main.c loader/loader.c

all:	brine

brine: main.o loader.o
	$(CC) $(CFLAGS) -o brine main.o loader.o

main.o: loader/main.c
	$(CC) $(CFLAGS) -c $(SRCS)

loader.o: loader/loader.c
	$(CC) $(CFLAGS) -c $(SRCS)

clean:
	rm -f *.o .depend*

install: brine
	cp brine /usr/local/bin

uninstall: $(BIN)/brine
	rm -rf $(BIN)/brine
