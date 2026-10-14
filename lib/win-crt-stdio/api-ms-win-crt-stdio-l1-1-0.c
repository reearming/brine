#include <stdio.h>
#include <stdint.h>
#include "api-ms-win-crt-stdio-l1-1-0.h"

static int _commode;
static int _fmode;

__attribute__((visibility("default")))
__attribute__((ms_abi))
void* __acrt_iob_func(unsigned index) {
    switch (index) {
        case 0: return stdin;
        case 1: return stdout;
        case 2: return stderr;
        default: return NULL;
    }
}

__attribute__((visibility("default")))
int *__p__commode(void) {
    return &_commode;
}

__attribute__((visibility("default")))
int *__p__fmode(void) {
    return &_fmode;
}

__attribute__((visibility("default"))) 
__attribute__((ms_abi))
int __stdio_common_vfprintf(const uint64_t options, const FILE *stream, const char *format, _locale_t const locale, const va_list arglist) {
    //fprintf((FILE *)stream, format);
    printf("[ STUB ] function %s coming soon\n", __func__);
    return 0;
}



