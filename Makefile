CC=		clang
CFLAGS=		-O2 -fno-strict-aliasing -Wall -Wextra -std=c17
CFLAGBIN=
CFLAGSLIB=	-shared -fPIC -fvisibility=hidden

LIBDIR=		usr/local/lib/brine
BINDIR=		usr/local/bin
OUT=		out
PREFIX=		/

BINSRCS=	loader/main.c loader/loader.c
BINOBJ=		${BINSRCS:T:R:S/$/.o/}

LIBS=	api-ms-win-crt-stdio-l1-1-0.dll.so \
		api-ms-win-crt-runtime-l1-1-0.dll.so \
		api-ms-win-crt-locale-l1-1-0.dll.so \
		api-ms-win-crt-heap-l1-1-0.dll.so \
		api-ms-win-crt-private-l1-1-0.dll.so \
		api-ms-win-crt-string-l1-1-0.dll.so \
		KERNEL32.dll.so \
		api-ms-win-crt-math-l1-1-0.dll.so \
		api-ms-win-crt-environment-l1-1-0.dll.so \
		brine.so \

.PATH: loader lib lib/win-crt-stdio lib/win-crt-runtime lib/win-crt-locale lib/win-crt-heap lib/win-crt-private lib/win-crt-string lib/KERNEL32 lib/win-crt-math \
	lib/win-crt-environment lib/brinelib

.PHONY: all clean install uninstall

all:	${OUT} brine ${LIBS}

${OUT}:
	mkdir -p ${OUT}

brine: ${BINOBJ}
	${CC} ${CFLAGS} ${CFLAGBIN} -o brine ${BINOBJ}

.SUFFIXES: .c .o
.c.o:
	${CC} ${CFLAGS} ${CFLAGBIN} -c ${.IMPSRC} -o ${.TARGET}


.NOTPARALLEL:
api-ms-win-crt-stdio-l1-1-0.dll.so: api-ms-win-crt-stdio-l1-1-0.c
api-ms-win-crt-runtime-l1-1-0.dll.so: api-ms-win-crt-runtime-l1-1-0.c
api-ms-win-crt-locale-l1-1-0.dll.so: api-ms-win-crt-locale-l1-1-0.c
api-ms-win-crt-heap-l1-1-0.dll.so: api-ms-win-crt-heap-l1-1-0.c
api-ms-win-crt-private-l1-1-0.dll.so: api-ms-win-crt-private-l1-1-0.c
api-ms-win-crt-string-l1-1-0.dll.so: api-ms-win-crt-string-l1-1-0.c
KERNEL32.dll.so: KERNEL32.c
api-ms-win-crt-math-l1-1-0.dll.so: api-ms-win-crt-math-l1-1-0.c
api-ms-win-crt-environment-l1-1-0.dll.so: api-ms-win-crt-environment-l1-1-0.c
brine.so: brine.c

${LIBS}:
	${CC} ${CFLAGS} ${CFLAGSLIB} ${.ALLSRC} -o ${.TARGET}

clean:
	rm -f *.o .depend* brine *.dll.so
	rm -rf ${OUT}

install: brine
	install -d ${PREFIX}${LIBDIR}
	install -m 755 brine ${PREFIX}${BINDIR}
	install -m 755 ${LIBS} ${PREFIX}${LIBDIR}

uninstall:
	rm -f ${PREFIX}${BINDIR}/brine
	rm -rf ${PREFIX}${LIBDIR}

