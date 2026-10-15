#include "../winerror.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <threads.h>
#include "../winnt.h"
#include <unistd.h>
#include <sys/mman.h>
#include "../memoryapi.h"

static thread_local uint32_t last_error;
LPTOP_LEVEL_EXCEPTION_FILTER unhandled_exception_filter;

uint32_t WIN_CDECL GetLastError(void) {
    return last_error;
}

void WIN_CDECL SetLastError(uint32_t error) {
    last_error = error;
}

int32_t WIN_CDECL UnhandledExceptionFilter(EXCEPTION_POINTERS *exception_info) {
    printf("[ STUB ] function %s coming soon", __func__);
    return 0;
}

LPTOP_LEVEL_EXCEPTION_FILTER WIN_CDECL SetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lp_top_level_exception_filter) {
    LPTOP_LEVEL_EXCEPTION_FILTER previous_unhandled_exception_filter = unhandled_exception_filter;
    unhandled_exception_filter = lp_top_level_exception_filter;
    return previous_unhandled_exception_filter;
    return NULL;
}

void WIN_CDECL Sleep(uint32_t time) {
    usleep(time * 1000);
}

int WIN_CDECL VirtualProtect(void *lp_address, size_t dw_size, uint32_t fl_new_protect, uint32_t *lpfl_old_protect) {
    if (!lp_address || dw_size == 0) {
        return 0;
    }

    uint32_t page_size = sysconf(_SC_PAGESIZE);

    uintptr_t address = (uintptr_t)lp_address;
    uintptr_t aligned_address = address & ~(page_size - 1);
    size_t alignment_offset = address - aligned_address;

    size_t aligned_size = (dw_size + alignment_offset + page_size - 1) & ~(page_size - 1);

    int prot;
    switch (fl_new_protect) {
        case WIN_PAGE_READONLY:
            prot = PROT_READ;
            break;
        case WIN_PAGE_READWRITE:
            prot = PROT_READ | PROT_WRITE;
            break;
        case WIN_PAGE_EXECUTE:
            prot = PROT_EXEC;
            break;
        case WIN_PAGE_EXECUTE_READ:
            prot = PROT_EXEC | PROT_READ;
            break;
        default:
            prot = PROT_NONE;
            break;
    }

    if (lpfl_old_protect) {
        *lpfl_old_protect = WIN_PAGE_READWRITE;
    }

    if (mprotect((void*)aligned_address, aligned_size, prot) != 0) {
        return 0;
    }

    return 1;

    return 0;
}



size_t WIN_CDECL VirtualQuery(const void *lp_address, MEMORY_BASIC_INFORMATION *lpBuffer, size_t dwLength) {
    printf("[ STUB ] function %s coming soon", __func__);
    return 0;
}


