#include "../../include/brine.h"
#include "../../include/ucrt/corecrt_stdio_config.h"
#include <math.h>
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
int __stdio_common_vfprintf(const uint64_t options, FILE *stream, const char *format, const void *locale, va_list arglist) {
    int result;
   
    char *normalized = api->normalize_format(format);
    va_list *args = api->format_win_va_to_sysv(normalized, arglist, 2, 0);
    result = vfprintf(stream, normalized, *args);
    free(args[0]->overflow_arg_area);
    free(args[0]->reg_save_area);
    free(args);
    free(normalized);
    
    return result;
}

WIN_CDECL
int __stdio_common_vfscanf(const uint64_t options, FILE *stream, const char *format, const void *locale, va_list arglist) {
    char *normalized = api->normalize_format(format);
    va_list *args = api->format_win_va_to_sysv(normalized, arglist, 2, 0);
    int result = vfscanf(stream, normalized, *args);
    free(args[0]->overflow_arg_area);
    free(args[0]->reg_save_area);
    free(args);
    return result;
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
    return 0;//api->host_setvbuf(stream, buf, mode, n);
}

__attribute__((alias("win_setvbuf"), visibility("default")))
int setvbuf(FILE *stream, char *buf, int mode, size_t n);

