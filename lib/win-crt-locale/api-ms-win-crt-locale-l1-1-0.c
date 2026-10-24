#include "../../include/brine.h"
#include "../../include/winnt.h"
#include <stdio.h>
#include <stdint.h>

static int thread_locale;

WIN_CDECL
int _configthreadlocale(int per_thread_locale_type) {
    int locale = thread_locale;
    thread_locale = locale;
    fprintf(stderr, "[ STUB ] function %s coming soon\n", __func__);
    return locale;
}



