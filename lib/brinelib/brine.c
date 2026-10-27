#include "../../include/brine.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <machine/sysarch.h>
#include <stdlib.h>
#include <string.h>

typedef struct brine_thread {
    void *sys_gs;
    void *teb;
} brine_thread;

static struct brine_thread thread;

struct brine_options options;
DEFAULT_ATTRIBUTES
void brinelib_init(struct brine_options settings) {
    options = settings;
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
    return &options.win_argc;
}

DEFAULT_ATTRIBUTES
char **get_argv() {
    return options.win_argv;
}

DEFAULT_ATTRIBUTES
char *normalize_format(const char *format) {
    size_t len = strlen(format) + 1;
    char *normalized = malloc(len);
    if (normalized == NULL)
        return NULL;

    memcpy(normalized, format, len);

    while (*normalized) {
        if (*normalized == '%') {
            normalized++;
            if (*normalized == 'L' && *(normalized + 1) == 'f') {
                *normalized = 'l';
                normalized++;
            }
        }
        normalized++;
    }
    normalized -= len - 1;
    return normalized;
}

DEFAULT_ATTRIBUTES
int define_format_overflow(const char *format, uint8_t first_unused_gp, uint8_t first_unused_fp) {
    int count = 0;
    while (*format) {
        if (*format == '%') {
            format++;
            if (*format == '%') {
                format++;
                continue;
            }
            switch (*format) {
                case 'f':
                case 'F':
                case 'g':
                case 'e':
                    if (first_unused_fp > 7) {
                        count += 8;
                    }
                    first_unused_fp++;
                    break;
                case '%':
                    break;

                case 'L':
                    format++;
                    if (*format == 'f' || *format == 'F' || *format == 'g' || *format == 'G' || *format == 'e' || *format == 'E' || *format == 'a' || *format == 'A') {
                        count += 16;
                    }
                    else {
                        if (first_unused_gp > 5) {
                            count += 8;
                        }
                        first_unused_gp++;
                    }

                    break;

                case 'l':
                    format++;
                    if (*format == 'f' || *format == 'F' || *format == 'g' || *format == 'G' || *format == 'e' || *format == 'E' || *format == 'a' || *format == 'A') {
                        if (first_unused_fp > 7) {
                            count += 8;
                        }
                        first_unused_fp++;
                    }
                    else {
                        if (first_unused_gp > 5) {
                            count += 8;
                        }
                        first_unused_gp++;
                    }

                    break;

                default:
                    if (first_unused_gp > 5) {
                        count += 8;
                    }
                    first_unused_gp++;
                    break;
            }
        }
        format++;
    }
    return count;
}

DEFAULT_ATTRIBUTES
va_list *format_win_va_to_sysv(const char *format, win_va_list arglist, uint8_t first_unused_gp, uint8_t first_unused_fp) {
    va_list *sysv_va_list = malloc(sizeof(va_list));
    uint8_t gp_offset = 8 * first_unused_gp;
    uint8_t fp_offset = 48 + first_unused_fp * 16;
    sysv_va_list[0]->gp_offset = gp_offset;
    sysv_va_list[0]->fp_offset = fp_offset;
    sysv_va_list[0]->reg_save_area = malloc(176);
    sysv_va_list[0]->overflow_arg_area = NULL;
    int overflow = define_format_overflow(format, first_unused_gp, first_unused_fp);
    if (overflow != 0) {
        sysv_va_list[0]->overflow_arg_area = malloc(overflow);
    }
    int overflow_arg_number = 0;
    while (*format) {
        if (*format == '%') {
            format++;
            if (*format == '%') {
                format++;
                continue;
            }
            switch (*format) {
                case 'f':
                case 'F':
                case 'g':
                case 'e':
                    if (fp_offset < 176) {
                        *(double *)(sysv_va_list[0]->reg_save_area + fp_offset) = *(double *)arglist;
                        fp_offset += 16;
                    }
                    else {
                        *(double *)(sysv_va_list[0]->overflow_arg_area + overflow_arg_number) = *(double *)arglist;
                        overflow_arg_number += 8;
                    }
                    arglist += 8;
                    break;
                case '%':
                    break;
                
                case 'L':
                    format++;
                    if (*format == 'f' || *format == 'F' || *format == 'g' || *format == 'G' || *format == 'e' || *format == 'E' || *format == 'a' || *format == 'A') {
                    
                        *(long double *)(sysv_va_list[0]->overflow_arg_area + overflow_arg_number) = **(long double **)arglist;
                        //long double value = **(long double **)arglist;
                        //memcpy((uint8_t *)sysv_va_list[0]->overflow_arg_area + overflow_arg_number, &value, sizeof(value));
                        overflow_arg_number += 16;
                    }
                    else {
                        if (gp_offset < 48) {
                            *(uint64_t *)(sysv_va_list[0]->reg_save_area + gp_offset) = *(uint64_t *)arglist;
                            gp_offset += 8;
                        }
                        else {
                            *(uint64_t *)(sysv_va_list[0]->overflow_arg_area + overflow_arg_number) = *(uint64_t *)arglist;
                            overflow_arg_number += 8;
                        }
                    }

                    arglist += 8;
                break;

                case 'l':
                    format++;
                    if (*format == 'f' || *format == 'F' || *format == 'g' || *format == 'G' || *format == 'e' || *format == 'E' || *format == 'a' || *format == 'A') {
                        if (fp_offset < 176) {
                            *(double *)(sysv_va_list[0]->reg_save_area + fp_offset) = *(double *)arglist;
                            fp_offset += 16;
                        }
                        else {
                            *(double *)(sysv_va_list[0]->overflow_arg_area + overflow_arg_number) = *(double *)arglist;
                            overflow_arg_number += 8;
                        }
                    }
                    else {
                        if (gp_offset < 48) {
                            *(uint64_t *)(sysv_va_list[0]->reg_save_area + gp_offset) = *(uint64_t *)arglist;
                            gp_offset += 8;
                        }
                        else {
                            *(uint64_t *)(sysv_va_list[0]->overflow_arg_area + overflow_arg_number) = *(uint64_t *)arglist;
                            overflow_arg_number += 8;
                        }
                    }

                    arglist += 8;
                break;

                default:
                    if (gp_offset < 48) {
                        *(uint64_t *)(sysv_va_list[0]->reg_save_area + gp_offset) = *(uint64_t *)arglist;
                        gp_offset += 8;
                    }
                    else {
                        *(uint64_t *)(sysv_va_list[0]->overflow_arg_area + overflow_arg_number) = *(uint64_t *)arglist;
                        overflow_arg_number += 8;
                    }
                    arglist += 8;
                break;
            }
            
        }
        format++;

    }
    return sysv_va_list;
}

