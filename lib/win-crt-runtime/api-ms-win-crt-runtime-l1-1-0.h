#ifndef API_MS_WIN_CRT_RUNTIME_L1_1_0_H
#define API_MS_WIN_CRT_RUNTIME_L1_1_0_H

#include <stdint.h>

typedef void (__cdecl *_PVFV)(void);
typedef int  (__cdecl *_PIFV)(void);

typedef enum {
    _crt_argv_no_arguments = 0,
    _crt_argv_unexpanded_arguments = 1,
    _crt_argv_expanded_arguments = 2,
} _crt_argv_mode;

typedef struct {

} exception_record;

typedef struct {

} context;

typedef struct {
    exception_record *exception_record;
    context *context_record;
} exception_pointers;

typedef enum _crt_app_type {
    _crt_unknown_app,
    _crt_console_app,
    _crt_gui_app
} _crt_app_type;

typedef void (__cdecl *_invalid_parameter_handler)(
    const uint16_t* expression,
    const uint16_t* function,
    const uint16_t* file,
    unsigned int line,
    uintptr_t pReserved
);

#endif
