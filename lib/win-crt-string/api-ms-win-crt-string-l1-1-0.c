#include "../../include/brine.h"
#include <stdio.h>


static brine_api *api;
DEFAULT_ATTRIBUTES 
void so_init(brine_api *brine_api) {
    api = brine_api;
}

WIN_CDECL 
size_t win_strlen(const char *str) {
    fprintf(stderr, "%s called\n", __func__);
    return api->host_strlen(str); 
}

__attribute__((alias("win_strlen"), visibility("default")))
size_t strlen(const char *str);
