#include "../../include/winnt.h"
#include "../../include/brine.h"
#include <stdio.h>

WIN_CDECL
int _matherr(struct _exception *except) {
    printf("[ STUB ] function %s coming soon\n", __func__);
    return 0;
}

_HANDLE_MATH_ERROR matherr_handle = _matherr;

WIN_CDECL
void __setusermatherr(_HANDLE_MATH_ERROR pf) {
    matherr_handle = pf;
}

