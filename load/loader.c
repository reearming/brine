#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include "loader.h"
#include <sys/stat.h>
#include <sys/mman.h>

int try_PE(int fd) {
    if (fd < 0) {
        printf("can't open the file");
        exit(1);
    } 

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        printf("can't get the file size");
        goto close;        
    }

    uint8_t *pe_file = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (pe_file == MAP_FAILED) {
        printf("load to ram failed\n");
        goto close;
    }
    close(fd);

    uint32_t pe_offset = *(uint32_t *)(pe_file + PE_HEADER_OFFSET);
    uint32_t *pe_sig = (uint32_t *)(pe_file + pe_offset);

    if (((*pe_sig >> 0) & 0xFF) != 0x50 || ((*pe_sig >> 8) & 0xFF) != 0x45 || ((*pe_sig >> 16) & 0xFF) != 0x00 || ((*pe_sig >> 24) & 0xFF) != 0x00) {
        printf("provided file doesn't contain PE signature (0x%x != 0x00004550)\n", *pe_sig);
        goto unmap;
    }

    struct COFF *coff_header = (struct COFF *)(pe_sig + 1);
    if (coff_header->machine != IMAGE_FILE_MACHINE_AMD64 && coff_header->machine != IMAGE_FILE_MACHINE_I386) {
        printf("only x86 supported, machine type is 0x%x\n", coff_header->machine);
        goto unmap;
    }

    if (coff_header->characteristics != IMAGE_FILE_EXECUTABLE_IMAGE) {
        printf("not an executable\n");
        goto unmap;
    }

    uint8_t *optional_header = (uint8_t *)(coff_header + 1);
    // standard fields
    uint16_t optional_magic = *(uint16_t *)optional_header;


    munmap(pe_file, sb.st_size);
    return 0;
unmap:
    munmap(pe_file, sb.st_size);
    exit(1);
close:
    close(fd);
    exit(1);
}
