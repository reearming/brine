#include "../../include/brine.h"
#include "../../include/winerror.h"
#include "../../include/winnt.h"
#include <stdio.h>
#include <stdlib.h>

int malloc_mode;

static brine_api *api;
DEFAULT_ATTRIBUTES 
void so_init(brine_api *brine_api) {
    api = brine_api;
}

WIN_CDECL 
void *win_memcpy(void *dst, const void *src, size_t n) {
    return api->host_memcpy(dst, src, n);
}

__attribute__((alias("win_memcpy"), visibility("default"))) 
void *memcpy(void* dst, const void* src, size_t n);

