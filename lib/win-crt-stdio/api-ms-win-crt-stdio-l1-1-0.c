#include "../../include/brine.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <locale.h>

static int _commode;
static int _fmode;

static brine_api *api;
DEFAULT_ATTRIBUTES 
void so_init(brine_api *brine_api) {
    api = brine_api;
}

WIN_CDECL
void* __acrt_iob_func(unsigned index) { 
    switch (index) {
        case 0: return stdin;
        case 1: return stdout;
        case 2: return stderr;
        default: return NULL;
    }
}

WIN_CDECL
int *__p__commode(void) {
    return &_commode;
}

WIN_CDECL
int *__p__fmode(void) {
    return &_fmode;
}

WIN_CDECL
int __stdio_common_vfprintf(const uint64_t options, const FILE *stream, const char *format, locale_t const locale, va_list arglist) {
    return vfprintf((FILE *)stream, format, arglist);
}

WIN_CDECL
int __stdio_common_vfscanf(const uint64_t options, FILE *stream, const char *format, locale_t const locale, va_list arglist) {
    va_list *args = api->va_list_win_to_sysv(1, arglist, 12, "asd", NULL);
    return vfscanf(stream, format, *args);
}

WIN_CDECL
int win_fflush(FILE *stream) {
    return api->host_fflush(stream);
}   

__attribute__((alias("win_fflush"), visibility("default")))
int fflush(FILE *stream);

WIN_CDECL
int win_setvbuf(FILE *stream, char *buf, int mode, size_t n)
{
    return api->host_setvbuf(stream, buf, mode, n);
}

__attribute__((alias("win_setvbuf"), visibility("default")))
int setvbuf(FILE *stream, char *buf, int mode, size_t n);

