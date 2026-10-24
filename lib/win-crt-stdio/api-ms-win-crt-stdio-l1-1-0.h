#ifndef API_MS_WIN_CRT_STDIO_L1_1_0_H
#define API_MS_WIN_CRT_STDIO_L1_1_0_H
#include <xlocale.h>

typedef locale_t _locale_t;

typedef int (*system_setvbuf)(FILE *restrict, char *restrict, int, size_t);

#endif
