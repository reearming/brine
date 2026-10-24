#include "../../include/brine.h"
#include "../../include/winnt.h"
#include <stdio.h>

char **_environ = NULL;
extern char **environ;

WIN_CDECL
char ***__p__environ() {
    fprintf(stderr, "%s called\n", __func__);
    if (!_environ) {
        _environ = environ;
    }
    return &_environ;
}
