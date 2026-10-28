#ifndef CORECRT_STDIO_CONFIG_H
#define CORECRT_STDIO_CONFIG_H
#include <stdint.h>

#define _CRT_INTERNAL_PRINTF_X87_LONG_DOUBLE    1ULL << 28

#ifdef __cplusplus
exextern "C" {
#endif

#define _CRT_INTERNAL_PRINTF_OPTIONS \
    (*__local_stdio_printf_options())

uint64_t __local_stdio_printf_options(void);


#ifdef __cpluslus
}
#endif

#endif
