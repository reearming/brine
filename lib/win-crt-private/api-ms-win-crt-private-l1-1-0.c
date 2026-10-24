#include "../winerror.h"
#include <stdio.h>
#include "../winnt.h"
#include <stdlib.h>

int malloc_mode;

void *WIN_CDECL win_memcpy(void* dst, const void* src, size_t n) {
    printf("we are in %s", __func__);
    return __builtin_memcpy(dst, src, n);
}

__attribute__((alias("win_memcpy"), visibility("default"))) 
void *memcpy(void* dst, const void* src, size_t n);

