#include "../../include/brine.h"
#include "../../include/winnt.h"
#include <stdio.h>

char **_environ = NULL;
extern char **environ;

static brine_api *api;
DEFAULT_ATTRIBUTES 
void so_init(brine_api *brine_api) {
    api = brine_api;
}

WIN_CDECL
char ***__p__environ() {
    if (!_environ) {
        _environ = environ;
    }
    return &_environ;
}
