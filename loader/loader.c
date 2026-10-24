#include <stddef.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include "loader.h"
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <dlfcn.h>

static int fd;
struct stat sb;
extern int debug;

uint16_t try_MZ() {
    uint16_t mz_sig;
    read(fd, &mz_sig, 2);
    return mz_sig;
}

uint32_t *try_PE(uint8_t *pe_file) {
    uint32_t *pe_offset = (uint32_t *)(pe_file + PE_HEADER_OFFSET);
    uint32_t *pe_sig = (uint32_t *)(pe_file + *pe_offset);
     
    return pe_sig;
}

uint8_t *map_file(int map_fd) {
    if (fstat(fd, &sb) == -1) {
        return NULL;
    }

    uint8_t *file = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, map_fd, 0);
    if (file == MAP_FAILED) {
        return NULL;
    }
    close(fd);
    return file;
}

struct coff_header *parse_coff(uint32_t *pe_sig) {
    struct coff_header *coff_header = (struct coff_header *)(pe_sig + 1);
    return coff_header;
}

void *parse_optional(struct coff_header *coff_header) {
    struct standard_fields *standard_fields = (struct standard_fields *)(coff_header + 1);
    return (void *)standard_fields;
}

struct section_table *parse_section_table(struct pe *pe, void *optional) {
    struct section_table *table = (struct section_table *)((uint8_t *)optional + pe->coff->size_of_optional_header);
    return table;
}

struct section_table *get_section(struct section_table *table, uint16_t tables_count, const char *name) {
    for (uint16_t i = 0; i < tables_count; i++) {
        if (strncmp((const char *)table[i].name, name, 8) == 0) {
            return &table[i];
        }
    }
    return NULL;
}

uint8_t *load_sections(uint8_t *raw_file, struct pe *pe) {
    uint32_t size = 0;
    uint32_t size_of_headers = 0;
    if (pe->optional_magic == PE32) {
        size = pe->optional.pe32->size_of_image;
        size_of_headers = pe->optional.pe32->size_of_headers;
    }
    else {
        size = pe->optional.pe32p->size_of_image;
        size_of_headers = pe->optional.pe32p->size_of_headers;
    }

    uint8_t *mapped_file = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (mapped_file == MAP_FAILED)
        return NULL;
    memcpy(mapped_file, raw_file, size_of_headers);
    for (uint32_t i = 0; i < pe->coff->number_of_sections; i++) {
        memcpy(mapped_file + pe->section_table[i].virtual_address, raw_file + pe->section_table[i].pointer_to_raw_data, pe->section_table[i].size_of_raw_data);
    }
    return mapped_file;
}

struct pe *parse(int f, uint8_t **file, int image_type) {
    fd = f; 

    uint16_t mz_sig = try_MZ();
    if (mz_sig != MZ_SIGNATURE) {
        printf("provided file doesn't contain MZ signature (0x%x != 0x%x)\n", mz_sig, MZ_SIGNATURE);
        exit_closefd(fd);
    }

    struct pe *pe = malloc(sizeof(struct pe));
    pe->mz_sig = mz_sig;
    uint8_t *raw_file = map_file(f);
    if (raw_file == NULL) {
        printf("load to ram failed\n");
        exit_closefd(fd);
    }

    *file = raw_file;

    uint32_t *pe_sig = try_PE(raw_file);
    if (*pe_sig != 0x00004550) {
        printf("provided file doesn't contain PE signature (0x%08x != 0x00004550)\n", *pe_sig);
        exit_unmap(raw_file, sb);
    }
    pe->pe_sig = pe_sig;

    struct coff_header *coff_header = parse_coff(pe_sig);
    pe->coff = coff_header;
    
    if (image_type == IMAGE_FILE_EXECUTABLE && !(coff_header->characteristics & IMAGE_FILE_EXECUTABLE)) {
        printf("not an executable\n");
        exit_unmap(raw_file, sb);
    }
    else if (image_type == IMAGE_FILE_DLL && !(coff_header->characteristics & IMAGE_FILE_DLL)) {
        printf("not a dll\n");
        exit_unmap(raw_file, sb);
    }

    void *optional_ptr = parse_optional(coff_header);
    uint16_t optional_magic = *(uint16_t *)optional_ptr;
    if (optional_magic == PE32) {
        pe->optional.pe32 = optional_ptr;
        pe->optional_magic = optional_magic;
    }
    else if (optional_magic == PE32P) {
        pe->optional.pe32p = optional_ptr;
        pe->optional_magic = optional_magic;
    }
    else {
        printf("optional header magic is undefined (0x%x != 0x%x || 0x%x != 0x%x)\n", optional_magic, PE32, optional_magic, PE32P);
        exit_unmap(raw_file, sb);
    }
    
    struct section_table *section_table = parse_section_table(pe, optional_ptr);
    pe->section_table = section_table; 
    
    return pe;
}

int load_dll(struct pe *pe, uint8_t *mapped_file, uint32_t *count, brine_api *api) {
    uint16_t optional_magic = pe->optional_magic; 

    if (optional_magic == PE32) {
        uint32_t idt_rva = pe->optional.pe32->data_directory[1].virtual_address;
        struct import_descriptor_table *idt = (struct import_descriptor_table *)(mapped_file + idt_rva);

        uint32_t ilt_rva = idt->ilt_rva;

        uint32_t *ILT = (uint32_t *)(mapped_file + ilt_rva);
        uint32_t ordinal = 0x80000000 & *ILT;
        
        if (ordinal) {

        }
        else {

        }
    }
    else {
        uint32_t idt_rva = pe->optional.pe32p->data_directory[1].virtual_address;

        struct import_descriptor_table *idt = (struct import_descriptor_table *)(mapped_file + idt_rva);
        for (int d = 0; idt[d].name_rva != 0; d++) {

            uint32_t lookup_rva = 0;
            if (idt[d].ilt_rva != 0)
                lookup_rva = idt[d].ilt_rva;
            else
                lookup_rva = idt[d].iat_rva;
            
            uint64_t *ILT = (uint64_t *)(mapped_file + lookup_rva);
            uint64_t *IAT = (uint64_t *)(mapped_file + idt[d].iat_rva);

            for (int i = 0; ILT[i] != 0; i++) {
                uint64_t ordinal = ILT[i] & 0x8000000000000000ULL;

                if (ordinal) {

                }
                else {
                    uint32_t hint_table_rva = ILT[i] & 0x7FFFFFFF;
                    uint8_t *hint_table = mapped_file + hint_table_rva;
                    char *name = (char *)(hint_table + 2);
                    char lib_path[MAXPATHLEN];
                    
                    long path_size = snprintf(lib_path, sizeof(lib_path), "%s%s.so", brine_lib_dir, (char *)(mapped_file + idt[d].name_rva));

                    if (path_size >= (long)sizeof(lib_path) || path_size < 0)
                        return -1;

                    void *lib = dlopen(lib_path, RTLD_LAZY | RTLD_DEEPBIND);
                    if (!lib) {
                        fprintf(stderr, "error loading %s\n", lib_path);
                        return -2;
                    }
                    dlerror();
                    /* we can let a windows application use any function with no args from FreeBSD libc*/
                    void *func = dlsym(lib, name);

                    char *error;
                    if ((error = dlerror()) != NULL) {
                        fprintf(stderr, "error finding function %s\n", name);
                        return -3;
                    }
                    
                    if (debug) {
                        Dl_info info = {0};

                        int result = dladdr(func, &info);

                        if (result != 0) {
                            //if (strstr(info.dli_fname, "libc")) {
                                printf("function %s resolved in: %s, requested in: %s\n", name, info.dli_fname, lib_path);
                            //}
                        }
                    }

                    IAT[i] = (uint64_t)func;
                    *count += 1;

                    void (*so_init)(brine_api *brine_api) = dlsym(lib, "so_init");
                    char *init_error;
                    if ((init_error = dlerror()) == NULL) {
                        so_init(api);
                    }
                }
            }
        }
    }

    return 0;
}

int base_relocation(uint8_t *mapped_file, struct pe *pe, uint64_t delta) {
    struct data_directory reloc_dir;

    if (pe->optional_magic == PE32) {
        reloc_dir = pe->optional.pe32->data_directory[5];
    }
    else {
        reloc_dir = pe->optional.pe32p->data_directory[5];
        
        uint8_t *reloc_ptr = mapped_file + reloc_dir.virtual_address;
        uint8_t *reloc_end = reloc_ptr + reloc_dir.size;
        
        while (reloc_ptr < reloc_end) {
            struct base_relocation_block *reloc = (struct base_relocation_block *)reloc_ptr;

            uint32_t entries_count = (reloc->block_size - sizeof(struct base_relocation_block)) / 2;

            uint16_t *entries = (uint16_t *)(reloc + 1);
            for (uint32_t i = 0; i < entries_count; i++) {
                uint16_t entry = entries[i];
                uint32_t reloc_type = entry >> 12;
                uint32_t offset = entry & 0x0FFF;

                if (reloc_type == IMAGE_REL_BASED_DIR64) {
                    uint64_t *target = (uint64_t *)(mapped_file + reloc->page_rva + offset);
                    *target += delta;
                }
            }
            
            reloc_ptr += reloc->block_size;
        }
    }

    return 0;
}

int mprotect_sections(struct pe *pe, uint8_t *mapped_file) {
    for (int i = 0; i < pe->coff->number_of_sections; i++) {
        struct section_table section = pe->section_table[i];
        if (strncmp((const char *)pe->section_table[i].name, ".rdata", 8) == 0 | strncmp((const char *)pe->section_table[i].name, ".reloc", 8) == 0) {
            if (mprotect(mapped_file + section.virtual_address, section.virtual_size, PROT_READ) < 0) {
                fprintf(stderr, "failed to mprotect %s to RO\n", section.name);
                return -1;
            }
        }
        else if (strncmp((const char *)pe->section_table[i].name, ".text", 8) == 0) {
            if (mprotect(mapped_file + section.virtual_address, section.virtual_size, PROT_READ | PROT_EXEC) < 0) {
                fprintf(stderr, "failed to mprotect %s to RX\n", section.name);
                return -2;
            }
        }
        
    }
    return 0;
}

brine_api *load_api() {
    void *brine_so = dlopen(brine_lib, RTLD_LAZY);
    
    void (*brine_write_thread)(brine_thread *) = dlsym(brine_so, "brine_write_thread");
    int (*brine_read_thread)(brine_thread **) = dlsym(brine_so, "brine_read_thread");
    int (*set_teb_gs)() = dlsym(brine_so, "set_teb_gs");
    int (*set_sys_gs)() = dlsym(brine_so, "set_teb_gs");

    brine_api *api = malloc(sizeof(struct brine_api));
    api->thread_write = brine_write_thread;
    api->thread_read = brine_read_thread;
    api->set_teb_gs = set_teb_gs;
    api->set_sys_gs = set_sys_gs;
    return api;
}
