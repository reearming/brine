#include "../winerror.h"
#include <stdio.h>
int malloc_mode;

__attribute__((visibility("default"))) 
__attribute__((ms_abi))
int _set_new_mode(int new_handler_mode) {
    if (new_handler_mode != 0 || new_handler_mode != 1) {
        printf("function %s returned error code %x (new_handler_mode can't be anything but 0 or 1)\n", __func__, FACILITY_STORAGE);
        return FACILITY_STORAGE;
    }

    int previous_mode = malloc_mode;
    malloc_mode = new_handler_mode;
    return previous_mode;
}



