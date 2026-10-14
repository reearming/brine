#include <stdio.h>
#include "api-ms-win-crt-runtime-l1-1-0.h"


int _argc;
int _argv;

__attribute__((visibility("default"))) 
int *__p___argc(void) {
    return &_argc;
}
__attribute__((visibility("default"))) 
int *__p___argv(void) {
    return &_argv;
}

__attribute__((visibility("default"))) 
void _cexit(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }

    fflush(NULL);

}

__attribute__((visibility("default"))) 
int __cdecl _configure_narrow_argv(_crt_argv_mode mode) {
    printf("[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

__attribute__((visibility("default"))) 
int __cdecl _crt_atexit(void (__cdecl *function)(void)) {
    printf("[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

__attribute__((visibility("default"))) 
int __cdecl _initialize_narrow_environment(void) {
    printf("[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

__attribute__((visibility("default"))) 
int __cdecl _initterm(_PVFV* pfbegin, _PVFV* pfend) {
    while (pfbegin < pfend) {
        if (pfbegin != 0)
            (**pfbegin)();
        pfbegin++;
    }
    return 0;
} 

__attribute__((visibility("default"))) 
int __cdecl _initterm_e(_PIFV* pfbegin, _PIFV* pfend)
{
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

__attribute__((visibility("default"))) 
int __cdecl _seh_filter_dll(unsigned long exception_num, exception_pointers* exception_ptr) {
    printf("[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

__attribute__((visibility("default"))) 
int __cdecl _seh_filter_exe(unsigned long exception_num, exception_pointers* exception_ptr) {
    printf("[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

__attribute__((visibility("default"))) 
void __cdecl _set_app_type(_crt_app_type appType) {
    printf("[ STUB ] function %s coming soon\n", __func__);
}

__attribute__((visibility("default"))) 
__attribute__((ms_abi))
_invalid_parameter_handler _set_invalid_parameter_handler(_invalid_parameter_handler pNew) {
    printf("[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

    

