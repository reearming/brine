#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include "loader.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "no arguments provided\n");
        return 1;
    } 
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "can't open the file\n");
        exit(1);
    }
    uint8_t *raw_file = 0;
    struct stat sb;
    struct pe *pe = parse(fd, &raw_file, &sb, IMAGE_FILE_EXECUTABLE);

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
        
        if (base_relocation(mapped_file, pe, reloc_delta) < 0) {
            fprintf(stderr, "failed to base relocation\n");
        } 
    }

    if (mprotect_sections(pe, mapped_file) < 0) {
        goto unmap;
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
