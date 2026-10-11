CC=		clang
CFLAGS=		-O2 -fno-strict-aliasing -Wall -Wextra -std=c17
CFLAGBIN=
CFLAGSLIB=	-shared -fPIC 

LIBDIR=	/usr/local/lib/brine
BINDIR=	/usr/local/bin

BINSRCS=	loader/main.c loader/loader.c 

BINOBJ=	main.o loader.o

LIBS=	test.dll.so
OUT=	out

all:	brine $(LIBS)

brine: bin_object
	$(CC) $(CFLAGS) $(CFLAGBIN) -o brine $(BINOBJ)

bin_object: $(BINSRCS)
	$(CC) $(CFLAGS) $(CFLAGBIN) -c $(BINSRCS)

test.dll.so: lib/test.c
	$(CC) $(CFLAGS) $(CFLAGSLIB) lib/test.c -o test.dll.so

clean:
	rm -f *.o .depend*

install: brine
	mkdir -p $(LIBDIR)
	cp brine $(BINDIR)
	cp $(LIBS) $(LIBDIR)

uninstall: $(BINDIR)/brine $(LIBDIR)
	rm -rf $(BINDIR)/brine $(LIBDIR)
