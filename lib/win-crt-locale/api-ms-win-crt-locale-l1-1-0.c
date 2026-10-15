#include <stdio.h>
#include <stdint.h>
#include "../winnt.h"

static int thread_locale;

int WIN_CDECL _configthreadlocale(int per_thread_locale_type) {
    int locale = thread_locale;
    thread_locale = locale;
    printf("[ STUB ] function %s coming soon\n", __func__);
    return locale;
}



