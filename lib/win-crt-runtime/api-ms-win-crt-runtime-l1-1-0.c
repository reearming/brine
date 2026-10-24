#include <stdio.h>
#include <stdlib.h>
#include "api-ms-win-crt-runtime-l1-1-0.h"
#include "../winnt.h"

int _argc;
int _argv;

int WIN_CDECL *__p___argc(void) {
    printf("[ STUB ] function %s coming soon", __func__);
    return &_argc;
}

int WIN_CDECL *__p___argv(void) {
    printf("[ STUB ] function %s coming soon", __func__);
    return &_argv;
}

void WIN_CDECL _cexit(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }

    fflush(NULL);

}

int WIN_CDECL _configure_narrow_argv(_crt_argv_mode mode) {
    printf("[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

int WIN_CDECL _crt_atexit(void (__cdecl *function)(void)) {
    printf("[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

int WIN_CDECL _initialize_narrow_environment(void) {
    printf("[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

int WIN_CDECL _initterm(_PVFV* pfbegin, _PVFV* pfend) {
    printf("we are in %s\n", __func__);
    while (pfbegin < pfend) {
        if (pfbegin != 0)
            (**pfbegin)();
        pfbegin++;
    }
    return 0;
} 

int WIN_CDECL _initterm_e(_PIFV* pfbegin, _PIFV* pfend)
{
    printf("we are in %s\n", __func__);
    while (pfbegin < pfend)
    {
        if (*pfbegin != 0)
        {
            int res = (**pfbegin)();
            if (res != 0)
            {
                return res;
            }
        }
        ++pfbegin;
    }
    return 0;
}

int WIN_CDECL _seh_filter_dll(unsigned long exception_num, exception_pointers* exception_ptr) {
    printf("[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

int WIN_CDECL _seh_filter_exe(unsigned long exception_num, exception_pointers* exception_ptr) {
    printf("[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

void WIN_CDECL _set_app_type(_crt_app_type appType) {
    printf("[ STUB ] function %s coming soon\n", __func__);
}

_invalid_parameter_handler WIN_CDECL _set_invalid_parameter_handler(_invalid_parameter_handler pNew) {
    printf("[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

extern void sys_exit(int status) __asm__("exit");
void WIN_CDECL win_exit(int status) {
    sys_exit(status);
}
__attribute__((alias("win_exit"), visibility("default")))
void exit(int status);

