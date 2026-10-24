#include <stdio.h>
#include <stdint.h>
#include "api-ms-win-crt-stdio-l1-1-0.h"
#include "../winnt.h"
#include <stdlib.h>
#include <dlfcn.h>

static int _commode;
static int _fmode;

void* WIN_CDECL __acrt_iob_func(unsigned index) {
    switch (index) {
        case 0: return stdin;
        case 1: return stdout;
        case 2: return stderr;
        default: return NULL;
    }
}

int WIN_CDECL *__p__commode(void) {
    printf("[ STUB ] function %s coming soon", __func__);
    return &_commode;
}

int WIN_CDECL *__p__fmode(void) {
    printf("[ STUB ] function %s coming soon", __func__);
    return &_fmode;
}

int WIN_CDECL __stdio_common_vfprintf(const uint64_t options, const FILE *stream, const char *format, _locale_t const locale, const va_list arglist) {
    //fprintf((FILE *)stream, format);
    printf("[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

extern int sys_fflush(FILE *stream) __asm__("fflush");
int WIN_CDECL win_fflush(FILE *stream) {
    return sys_fflush(stream);
}   
__attribute__((alias("win_fflush"), visibility("default")))
int fflush(FILE *stream);

int WIN_CDECL win_setvbuf(FILE *stream, char *buf, int mode, size_t n)
{
    system_setvbuf host = (system_setvbuf)dlsym(dlopen("libc.so.7", RTLD_LAZY), "setvbuf");

    return host(stream, buf, mode, n);
}

__attribute__((alias("win_setvbuf"), visibility("default")))
int setvbuf(FILE *stream, char *buf, int mode, size_t n);

