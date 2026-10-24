#include "../../include/brine.h"
#include <stdio.h>
#include <stdlib.h>
#include "api-ms-win-crt-runtime-l1-1-0.h"

int __argc;
char **__argv;

static struct brine_api *api;

DEFAULT_ATTRIBUTES
void so_init(struct brine_api *brine_api)
{
    api = brine_api;
    __argc = *api->get_argc();
    __argv = api->get_argv();
}

WIN_CDECL 
int *__p___argc(void) {
    fprintf(stderr, "%s called\n", __func__);
    /*if (!_argc) {
        _argc = argc;
    }*/
    return &__argc;
}

WIN_CDECL
char ***__p___argv(void) {
    fprintf(stderr, "%s called\n", __func__);
    /*if (!_argv) {
        _argv = argv;
    }*/
    return &__argv;
}

WIN_CDECL
void _cexit(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }

    fflush(NULL);

}

WIN_CDECL 
int _configure_narrow_argv(_crt_argv_mode mode) {
    fprintf(stderr, "[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

WIN_CDECL
int _crt_atexit(void (__cdecl *function)(void)) {
    fprintf(stderr, "[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

WIN_CDECL
int _initialize_narrow_environment(void) {
    fprintf(stderr, "[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

WIN_CDECL
int _initterm(_PVFV* pfbegin, _PVFV* pfend) {
    fprintf(stderr, "%s called\n", __func__);
    while (pfbegin < pfend) {
        if (*pfbegin != 0)
            (*pfbegin)();
        pfbegin++;
    }
    return 0;
} 

WIN_CDECL
int _initterm_e(_PIFV* pfbegin, _PIFV* pfend)
{
    fprintf(stderr, "%s called\n", __func__);
    while (pfbegin < pfend)
    {
        if (*pfbegin != 0)
        {
            int res = (*pfbegin)();
            if (res != 0)
            {
                return res;
            }
        }
        ++pfbegin;
    }
    return 0;
}

WIN_CDECL
int _seh_filter_dll(unsigned long exception_num, exception_pointers* exception_ptr) {
    fprintf(stderr, "[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

WIN_CDECL
int _seh_filter_exe(unsigned long exception_num, exception_pointers* exception_ptr) {
    fprintf(stderr, "[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

WIN_CDECL 
void _set_app_type(_crt_app_type appType) {
    fprintf(stderr, "[ STUB ] function %s coming soon\n", __func__);
}

WIN_CDECL
_invalid_parameter_handler _set_invalid_parameter_handler(_invalid_parameter_handler pNew) {
    fprintf(stderr, "[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

extern void sys_exit(int status) __asm__("exit");
void WIN_CDECL win_exit(int status) {
    sys_exit(status);
}
__attribute__((alias("win_exit"), visibility("default")))
void exit(int status);

