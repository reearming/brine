#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include "loader.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <machine/sysarch.h>

uint8_t debug = 0;
extern struct stat sb;

char *parse_args(int argc, char *argv[]) {
    char *exefile = NULL;
    uint8_t is_flag = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case '-':
                    if (strcmp(argv[i], "--debug") == 0)
                        debug = 1;
                    break;
                case 'd':
                    debug = 1;
                    break;
                default:
                    fprintf(stderr, "invalid argument '%s'\n", argv[i]);
                    return NULL;
            }
        }
        else if (is_flag) {
            
        }
        else {
            exefile = argv[i];
        }
    }

    return exefile;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "no arguments provided\n");
        return 1;
    }
    char *exefile = parse_args(argc, argv);
    if (exefile == NULL) {
        exit(1);
    }

    int fd = open(exefile, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "can't open the file\n");
        exit(1);
    }
    uint8_t *raw_file = 0;
    struct pe *pe = parse(fd, &raw_file, IMAGE_FILE_EXECUTABLE);

    uint32_t entry_point = 0;
    uint32_t image_size = 0;
    struct data_directory *dir = 0;
    uint64_t base_image;
    if (pe->optional_magic == PE32) {
        entry_point = pe->optional.pe32->address_of_entry_point;
        image_size = pe->optional.pe32->size_of_image;
        dir = pe->optional.pe32->data_directory;
        base_image = pe->optional.pe32->image_base;
    } 
    else {
        entry_point = pe->optional.pe32p->address_of_entry_point;
        image_size = pe->optional.pe32p->size_of_image;
        dir = pe->optional.pe32p->data_directory;
        base_image = pe->optional.pe32p->image_base;
    }
    
    uint8_t *mapped_file = load_sections(raw_file, pe);

    if (mapped_file == NULL) {
        fprintf(stderr, "failed to load sections\n"); 
        goto unmap;
    }

    uint32_t dll_count = 0;
    
    if (dir[1].size != 0)
    {
        int dllerr = load_dll(pe, mapped_file, &dll_count);
        if (dllerr < 0) {
            fprintf(stderr, "failed to load dll\n");
            goto unmap;
        }
    }
    struct section_table *reloc = get_section(pe->section_table, pe->coff->number_of_sections, ".reloc");
    if (reloc != NULL)
    {
        uint64_t reloc_delta = (uint64_t)mapped_file - base_image;
        if (reloc_delta != 0) {
            if (base_relocation(mapped_file, pe, reloc_delta) < 0) {
                fprintf(stderr, "failed to base relocation\n");
            } 
        }
    }
    
    if (mprotect_sections(pe, mapped_file) < 0) {
        goto unmap;
    }
    
    if (pe->optional_magic == PE32) {

    }
    else {
        uint8_t *teb = malloc(0x4000);
        uint8_t *peb = malloc(0x2000);
        *(teb + 0x30) = (uint64_t)teb;
        *(teb + 0x60) = (uint64_t)peb;
        if (amd64_set_gsbase(teb) != 0) {
            fprintf(stderr, "failed to set gs base\n");
        }
    }

    uint8_t *entry = mapped_file + entry_point;
    
    typedef void (*entry_t)(void);
    entry_t run_payload = (entry_t)entry;
    run_payload();

    munmap(mapped_file, image_size);
    exit_unmap(raw_file, sb);
    return 0;
unmap:
    munmap(mapped_file, image_size);
    exit_unmap(raw_file, sb);
    return -1;
}
