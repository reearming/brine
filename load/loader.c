#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include "loader.h"
#include <sys/stat.h>
#include <sys/mman.h>

static int fd;
static struct stat sb;

uint16_t try_MZ() {
    uint16_t mz_sig;
    read(fd, &mz_sig, 2);
    if (mz_sig != MZ_SIGNATURE) {
        printf("provided file doesm't contain MZ signature (0x%x != 0x%x)\n", mz_sig, MZ_SIGNATURE);
        exit_closefd(fd);
    }
    return mz_sig;
}

uint32_t try_PE(uint8_t *pe_file) {
    uint32_t pe_offset = *(uint32_t *)(pe_file + PE_HEADER_OFFSET);
    uint32_t *pe_sig = (uint32_t *)(pe_file + pe_offset);

    if (((*pe_sig >> 0) & 0xFF) != 0x50 || ((*pe_sig >> 8) & 0xFF) != 0x45 || ((*pe_sig >> 16) & 0xFF) != 0x00 || ((*pe_sig >> 24) & 0xFF) != 0x00) {
        printf("provided file doesn't contain PE signature (0x%x != 0x00004550)\n", *pe_sig);
        exit_unmap(pe_file, sb);
    }
    return *pe_sig;
}

uint8_t *map_ram() {
    if (fstat(fd, &sb) == -1) {
        printf("can't get the file size");
        exit_closefd(fd);
    }

    uint8_t *pe_file = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (pe_file == MAP_FAILED) {
        printf("load to ram failed\n");
        exit_closefd(fd);
    }
    close(fd);
    return pe_file;
}

struct coff_header parse_coff(uint8_t *pe_file, uint32_t *pe_sig) {
    struct coff_header coff_header = *(struct coff_header *)(pe_sig + 1);
    /*if (coff_header.machine != IMAGE_FILE_MACHINE_AMD64 && coff_header.machine != IMAGE_FILE_MACHINE_I386) {
        printf("only x86 supported, machine type is 0x%x\n", coff_header.machine);
        exit_unmap(pe_file, sb);
    }*/

    if (coff_header.characteristics != IMAGE_FILE_EXECUTABLE_IMAGE) {
        printf("not an executable\n");
        exit_unmap(pe_file, sb);
    }
    return coff_header;
}

void *parse_optional(struct coff_header *coff_header) {
    void *optional;
    struct standard_fields *standard_fields = (struct standard_fields *)(coff_header + 1);
    printf("%x\n", standard_fields->magic);
    optional = (void *)standard_fields;
    return optional;
}

struct pe *parse(int f) {
    fd = f;
    if (fd < 0) {
        printf("can't open the file");
        exit(1);
    }

    uint16_t mz_sig = try_MZ();
    struct pe *pe = malloc(sizeof(struct pe));
    pe->mz_sig = mz_sig;
    uint8_t *pe_file = map_ram();

    uint32_t pe_sig = try_PE(pe_file);
    pe->pe_sig = pe_sig;

    struct coff_header coff_header = parse_coff(pe_file, &pe_sig);
    pe->coff = coff_header;

    void *optional_ptr = parse_optional(&coff_header);
    if (((struct standard_fields *)optional_ptr)->magic == PE32P) {
        pe->optional.pe32p = *(struct pe32p_optional *)optional_ptr;
    }
    else if (((struct standard_fields *)optional_ptr)->magic == PE32) {
        pe->optional.pe32 = *(struct pe32_optional *)optional_ptr;
    }

    printf("%x\n", pe->optional.pe32p.standard_fields.magic);

    return pe;
}
