#include <stddef.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
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

uint32_t *try_PE(uint8_t *pe_file) {
    uint32_t *pe_offset = (uint32_t *)(pe_file + PE_HEADER_OFFSET);
    //uint32_t pe_offset = 0;
    //memcpy(&pe_offset, pe_file + PE_HEADER_OFFSET, sizeof(uint32_t));
    
    uint32_t *pe_sig = (uint32_t *)(pe_file + *pe_offset);
    //uint32_t *pe_sig = 0;
    //memcpy(pe_sig, pe_file + pe_offset, sizeof(uint32_t));    

    //if (((pe_sig >> 0) & 0xFF) != 0x50 || ((pe_sig >> 8) & 0xFF) != 0x45 || ((pe_sig >> 16) & 0xFF) != 0x00 || ((pe_sig >> 24) & 0xFF) != 0x00) {
    if (*pe_sig != 0x00004550) {
        printf("provided file doesn't contain PE signature (0x%08x != 0x00004550)\n", *pe_sig);
        exit_unmap(pe_file, sb);
    }
    return pe_sig;
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

struct coff_header *parse_coff(uint8_t *pe_file, uint32_t *pe_sig) {
    struct coff_header *coff_header = (struct coff_header *)(pe_sig + 1);
    if (coff_header->machine != IMAGE_FILE_MACHINE_AMD64 && coff_header->machine != IMAGE_FILE_MACHINE_I386) {
        printf("only x86 supported, machine type is 0x%x\n", coff_header->machine);
        exit_unmap(pe_file, sb);
    }

    if (!(coff_header->characteristics & IMAGE_FILE_EXECUTABLE_IMAGE)) {
        printf("not an executable\n");
        exit_unmap(pe_file, sb);
    }
    return coff_header;
}

void *parse_optional(struct coff_header *coff_header) {
    void *optional;
    struct standard_fields *standard_fields = (struct standard_fields *)(coff_header + 1);
    optional = (void *)standard_fields;
    return optional;
}

struct section_table *parse_section_table(void *optional) {
    if (*(uint16_t *)optional == PE32) {
        struct section_table *table = (struct section_table *)((struct pe32_optional *)optional + 1);
        return table;
    }
    else {
        struct section_table *table = (struct section_table *)((struct pe32p_optional *)optional + 1);
        return table;
    }
    return NULL;
}

struct section_table *get_section(struct section_table *table, uint16_t tables_count, const char *name) {
    for (uint16_t i = 0; i < tables_count; i++) {
        if ((const char *)table[i].name == name) {
            return &table[i];
        }
    }
    return NULL;
}

void *load_section(const void* section, size_t size, int loadprot, int prot, int flags) {
    int fd = shm_open(SHM_ANON, O_RDWR, 0600);
    ftruncate(fd, size);

    void *wptr = mmap(NULL, size, loadprot, flags, fd, 0);
    memcpy(wptr, section, size);

    void *ptr = mmap(NULL, size, prot, flags, fd, 0);
    munmap(wptr, size);
    close(fd);

    return ptr;
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

    uint32_t *pe_sig = try_PE(pe_file);
    pe->pe_sig = pe_sig;

    struct coff_header *coff_header = parse_coff(pe_file, pe_sig);
    pe->coff = coff_header;

    void *optional_ptr = parse_optional(coff_header);
    if (((struct standard_fields *)optional_ptr)->magic == PE32P) {
        pe->optional.pe32p = (struct pe32p_optional *)optional_ptr;
    }
    else if (((struct standard_fields *)optional_ptr)->magic == PE32) {
        pe->optional.pe32 = (struct pe32_optional *)optional_ptr;
    }
    else {
        printf("optional header magic is undefined (0x%x != 0x%x || 0x%x != 0x%x)", *(uint16_t *)optional_ptr, PE32, *(uint16_t *)optional_ptr, PE32P);
    }
    
    struct section_table *section_table = parse_section_table(optional_ptr);
    
    struct section_table *text = get_section(section_table, coff_header->number_of_sections, ".text");

    void *text_ptr = load_section(pe_file + text->virtual_address, text->virtual_size, PROT_READ | PROT_WRITE, PROT_READ | PROT_EXEC, MAP_SHARED);
    
    void *entry_point = text_ptr + (pe->optional.pe32p->standard_fields.address_of_entry_point - text->virtual_address);

    ((void(*)())entry_point)();

    printf("test\n");
    return pe;
}
