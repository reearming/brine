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
        printf("no arguments provided\n");
        return 1;
    } 
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        printf("can't open the file\n");
        exit(1);
    }
    uint8_t *raw_file = 0;
    struct stat sb;
    struct pe *pe = parse(fd, &raw_file, &sb, IMAGE_FILE_EXECUTABLE);

    uint32_t entry_point = 0;
    uint32_t image_size = 0;
    struct data_directory *dir = 0;
    if (pe->optional_magic == PE32) {
        entry_point = pe->optional.pe32->address_of_entry_point;
        image_size = pe->optional.pe32->size_of_image;
        dir = pe->optional.pe32->dir;
    } 
    else {
        entry_point = pe->optional.pe32p->address_of_entry_point;
        image_size = pe->optional.pe32p->size_of_image;
        dir = pe->optional.pe32p->dir;
    }
    
    uint8_t *mapped_file = load_sections(raw_file, pe);

    if (mapped_file == NULL) {
        printf("failed to load sections\n");
        munmap(mapped_file, image_size);
        exit_unmap(raw_file, sb);
    }

    uint32_t dll_count = 0;
    if (dir[1].virtual_address == 0)
         load_dll(pe, mapped_file, &dll_count);
     
    /*uint8_t *entry = allocated + entry_point;
    typedef void (*entry_t)(void);
    entry_t run_payload = (entry_t)entry;
    run_payload();*/
    munmap(mapped_file, image_size);
    exit_unmap(raw_file, sb);
    return 0;
}
