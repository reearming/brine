#include "../winnt.h"
#include <stdio.h>

char **_environ;

char WIN_CDECL ***__p__environ() {
    printf("[ STUB ] function %s coming soon", __func__);
    return &_environ;
}
