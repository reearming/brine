#include "../winerror.h"
#include <stddef.h>
#include <stdio.h>
#include "../winnt.h"

int malloc_mode;

int WIN_CDECL _set_new_mode(int new_handler_mode) {
    if (new_handler_mode != 0 && new_handler_mode != 1) {
        printf("function %s returned error code %x (new_handler_mode can't be anything but 0 or 1)\n", __func__, FACILITY_STORAGE);
        return FACILITY_STORAGE;
    }

    int previous_mode = malloc_mode;
    malloc_mode = new_handler_mode;
    return previous_mode;
}

void *WIN_CDECL win_malloc(size_t size) {
    return __builtin_malloc(size);
}
__attribute__((alias("win_malloc"), visibility("default")))
void *malloc(size_t size);

