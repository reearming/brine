CC=		clang
CFLAGS=	-O2 -fno-strict-aliasing -Wall -Wextra

SRCS=	load/main.c load/loader.c

all:	brine

brine: main.o loader.o
	$(CC) $(CFLAGS) -o brine main.o loader.o

main.o: load/main.c
	$(CC) $(CFLAGS) -c $(SRCS)

loader.o: load/loader.c
	$(CC) $(CFLAGS) -c $(SRCS)

clean:
	rm -f *.o .depend* 


