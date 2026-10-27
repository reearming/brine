#ifndef BRINE_H
#define BRINE_H
#define DEFAULT_ATTRIBUTES __attribute__((visibility("default")))
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#ifdef __x86_64__
#define WIN_CDECL __attribute__((ms_abi, visibility("default")))
#define WIN_ABI __attribute__((ms_abi))
#elif __i386__
#define WIN_CDECL __attribute__((cdecl, visibility("default")))
#define WIN_ABI __attribute__((cdecl))
#endif

typedef struct brine_thread brine_thread;

typedef char *win_va_list;

//void brine_write_thread(void *sys_gs, void *teb);
//brine_thread *brine_read_thread();

typedef enum target_mode { //they have some difference, for example long double in msvc is 8 bytes but in mingw it is 16 bytes
    MSVCx64,
    MINGWx86_64, 
} target_mode;

typedef struct brine_options {
    target_mode target;
    int win_argc;
    char **win_argv;
} brine_options;

typedef struct brine_api {
    //brine functions
    void (*thread_write)(brine_thread *);
    int (*thread_read)(brine_thread **);
    int (*set_teb_gs)();
    int (*set_sys_gs)();
    int *(*get_argc)();
    char **(*get_argv)();
    va_list *(*format_win_va_to_sysv)(const char *format, va_list arglist, uint8_t first_unused_gp, uint8_t first_unused_fp); 
    char *(*normalize_format)(const char *format);

    //host functions
    void *(*host_malloc)(size_t);
    size_t (*host_strlen)(const char *s);
    void *(*host_memcpy)(void *dst, const void *src, size_t n);
    int (*host_setvbuf)(FILE *stream, char *buf, int modes, size_t n);
    void (*host_exit)(int status);
    int (*host_fflush)(FILE *stream);
    void (*host_free)(void *ptr);

    //brine parameters
    uint8_t debug;
    enum target_mode target;
} brine_api;

int set_sys_gs();
int set_teb_gs();

#endif
