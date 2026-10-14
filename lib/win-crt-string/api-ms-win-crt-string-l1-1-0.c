#include "../winerror.h"
#include <stdio.h>

__attribute__((visibility("default"))) 
__attribute__((ms_abi))
size_t strlen(const char *str) {
    return __builtin_strlen(str);
}


