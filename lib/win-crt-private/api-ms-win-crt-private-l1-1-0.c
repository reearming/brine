#include "../winerror.h"
#include <stdio.h>

int malloc_mode;

__attribute__((visibility("default"))) 
__attribute__((ms_abi))
void* memcpy(void* dst, const void* src, size_t n) {
    return __builtin_memcpy(dst, src, n);
}


