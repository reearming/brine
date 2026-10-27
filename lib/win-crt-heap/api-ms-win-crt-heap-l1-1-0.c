#include "../../include/brine.h"
#include "../../include/winerror.h"
#include "../../include/winnt.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int malloc_mode;

static brine_api *api;
DEFAULT_ATTRIBUTES 
void so_init(brine_api *brine_api) {
    api = brine_api;
}

WIN_CDECL 
int _set_new_mode(int new_handler_mode) {
    if (new_handler_mode != 0 && new_handler_mode != 1) {
        fprintf(stderr, "function %s returned error code %x (new_handler_mode can't be anything but 0 or 1)\n", __func__, FACILITY_STORAGE);
        return FACILITY_STORAGE;
    }

    int previous_mode = malloc_mode;
    malloc_mode = new_handler_mode;
    return previous_mode;
}

WIN_ABI
void *win_malloc(size_t size) {
    return api->host_malloc(size);
}
__attribute__((alias("win_malloc"), visibility("default")))
void *malloc(size_t size);

WIN_ABI
void win_free(void *ptr) {
    api->host_free(ptr);
}
__attribute__((alias("win_free"), visibility("default")))
void free(void *ptr);

