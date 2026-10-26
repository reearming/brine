#include "../../include/brine.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <machine/sysarch.h>
#include <stdlib.h>

typedef struct brine_thread {
    void *sys_gs;
    void *teb;
} brine_thread;

static struct brine_thread thread;
static int __argc;
static char **__argv;

DEFAULT_ATTRIBUTES
void brinelib_init(int win_argc, char **win_argv) {
    __argc = win_argc;
    __argv = win_argv;
}

DEFAULT_ATTRIBUTES
void brine_write_thread(brine_thread *new_thread) {
    thread.sys_gs = new_thread->sys_gs;
    thread.teb = new_thread->teb;
}

DEFAULT_ATTRIBUTES
int *brine_read_thread(brine_thread **brine_thread) {
    *brine_thread = &thread;
    return 0;
}

DEFAULT_ATTRIBUTES
int set_teb_gs() {
    if (amd64_set_gsbase(thread.teb) != 0) {
        fprintf(stderr, "failed to set TEB gs base\n");
        return -1;
    }
    return 0;
}

DEFAULT_ATTRIBUTES
int set_sys_gs() {
    if (amd64_set_gsbase(thread.sys_gs) != 0) {
        //fprintf(stderr, "failed to set FreeBSD gs\n"); we can't use printf without FreeBSD gs
        return -1;
    }
    return 0;
}

DEFAULT_ATTRIBUTES
int *get_argc() {
    return &__argc;
}

DEFAULT_ATTRIBUTES
char **get_argv() {
    return __argv;
}

DEFAULT_ATTRIBUTES
int define_format_argc(const char *format) {
    int count = 0;
    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%' && format[i+1] != '%')
            count++;
    }
    return count;
}

DEFAULT_ATTRIBUTES
va_list *win_va_to_sysv(int argc, win_va_list arglist, uint8_t first_unused_gp) {
    va_list *sysv_va_list = malloc(sizeof(va_list));
    uint8_t gp_offset = 8 * first_unused_gp;
    sysv_va_list[0]->gp_offset = gp_offset;
    sysv_va_list[0]->reg_save_area = malloc(48);
    if (argc + first_unused_gp > 6) {
        sysv_va_list[0]->overflow_arg_area = malloc((argc - first_unused_gp) * 8);
    }
    int j = 0;
    for (int i = 0; i < argc; i++) {
        if (gp_offset < 48) {
            *(uint64_t *)(sysv_va_list[0]->reg_save_area + gp_offset) = *(uint64_t *)arglist;
            gp_offset += 8;
        }
        else {
            *(uint64_t *)(sysv_va_list[0]->overflow_arg_area + j) = *(uint64_t *)arglist;
            j += 8;
        }
        arglist += 8;
    }
    return sysv_va_list;
}

