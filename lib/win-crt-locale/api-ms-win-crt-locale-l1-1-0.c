#include <stdio.h>
#include <stdint.h>

static int thread_locale;

__attribute__((visibility("default"))) 
__attribute__((ms_abi))
int _configthreadlocale(int per_thread_locale_type) {
    int locale = thread_locale;
    thread_locale = locale;
    printf("[ STUB ] function %s coming soon\n", __func__);
    return locale;
}



