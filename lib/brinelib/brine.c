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
va_list *va_list_win_to_sysv(int argc, va_list arglist, ...) {
    va_list args;
    va_start(args, NULL);
    printf("windows app:\n");
    printf("fp_offset %02x\n", arglist[0].fp_offset);
    printf("gp_offset %02x\n", arglist[0].gp_offset);
    printf("overflow_arg_area %p, value: %lx\n", arglist[0].overflow_arg_area, *(uint64_t *)arglist[0].overflow_arg_area);
    printf("reg_save_area %p\n", arglist[0].reg_save_area);
    for (int i = 0; i < 3; i++) {
        printf("\nsysv:\n");
        printf("fp_offset %02x\n", args[i].fp_offset);
        printf("gp_offset %02x\n", args[i].gp_offset);
        printf("overflow_arg_area %p, value: %lx\n", args[i].overflow_arg_area, *(uint64_t *)args[i].overflow_arg_area);
        printf("reg_save_area %p, value: %lx\n", args[i].reg_save_area, *(uint64_t *)args[i].reg_save_area);
    }
    exit(0);
}

