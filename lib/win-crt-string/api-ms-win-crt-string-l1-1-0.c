#include "../winerror.h"
#include <stdio.h>
#include "../winnt.h"

size_t WIN_CDECL win_strlen(const char *str) {
    return __builtin_strlen(str);
}

__attribute__((alias("win_strlen"), visibility("default")))
size_t strlen(const char *str);
