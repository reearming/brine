#ifndef BRINE_H
#define BRINE_H
#define DEFAULT_ATTRIBUTES __attribute__((visibility("default")))
#include <stdlib.h>
#include <stdio.h>

#ifdef __x86_64__
#define WIN_CDECL __attribute__((ms_abi, visibility("default")))
#define WIN_ABI __attribute__((ms_abi))
#elif __i386__
#define WIN_CDECL __attribute__((cdecl, visibility("default")))
#define WIN_ABI __attribute__((cdecl))
#endif

typedef struct brine_thread brine_thread;

//void brine_write_thread(void *sys_gs, void *teb);
//brine_thread *brine_read_thread();

typedef struct brine_api {
    void (*thread_write)(brine_thread *);
    int (*thread_read)(brine_thread **);
    int (*set_teb_gs)();
    int (*set_sys_gs)();
    int *(*get_argc)();
    char **(*get_argv)();
    void *(*host_malloc)(size_t);
    size_t (*host_strlen)(const char *);
    void *(*host_memcpy)(void *, const void *, size_t);
    int (*host_setvbuf)(FILE *, char *, int, size_t);
    void (*host_exit)(int status);
} brine_api;

int set_sys_gs();
int set_teb_gs();

#endif
