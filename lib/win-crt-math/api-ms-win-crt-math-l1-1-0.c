#include <stdio.h>
#include "../winnt.h"

int WIN_CDECL _matherr(struct _exception *except) {
    printf("[ STUB ] function %s coming soon", __func__);
    return 0;
}

_HANDLE_MATH_ERROR matherr_handle = _matherr;

void WIN_CDECL __setusermatherr(_HANDLE_MATH_ERROR pf) {
    matherr_handle = pf;
}

