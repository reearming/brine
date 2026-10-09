#ifndef LOADER_H
#define LOADER_H

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define MZ_SIGNATURE 0x5A4D
#define PE_HEADER_OFFSET 0x3C
#define PE32 0x10b
#define PE32P 0x20b
#define IMAGE_FILE_MACHINE_AMD64 0x8664
#define IMAGE_FILE_MACHINE_I386 0x14c
#define IMAGE_FILE_EXECUTABLE 0x0002
#define IMAGE_FILE_DLL 0x2000

static const char *lib_dir = "/usr/local/lib/brine/";

static inline void exit_unmap(uint8_t *file, struct stat sb) {
    munmap(file, sb.st_size);
    exit(1);
}

static inline void exit_closefd(int fd) {
    close(fd);
    exit(1);
}

struct coff_header {
    uint16_t machine;
    uint16_t number_of_sections;
    uint32_t time_date_stamp;
    uint32_t pointer_to_symbol_table;
    uint32_t number_of_symbols;
    uint16_t size_of_optional_header;
    uint16_t characteristics;
};

struct data_directory {
    uint16_t virtual_address;
    uint16_t size;
};

struct pe32_optional {
    // standard_fields
    uint16_t magic;
    uint8_t major_linker_version;
    uint8_t minor_linker_version;
    uint32_t size_of_code;
    uint32_t size_of_initialized_data;
    uint32_t size_of_uninitialized_data;
    uint32_t address_of_entry_point;
    uint32_t base_of_code;
    uint32_t base_of_data;
    
    // specific_fields
    uint32_t image_base;
    uint32_t section_alignment;
    uint32_t file_alignment;
    uint16_t major_os_version;
    uint16_t minor_os_version;
    uint16_t major_image_version;
    uint16_t minor_image_version;
    uint16_t major_subsystem_version;
    uint16_t minor_subsystem_version;
    uint32_t win32_version_value;
    uint32_t size_of_image;
    uint32_t size_of_headers;
    uint32_t checksum;
    uint16_t subsystem;
    uint16_t dll_characteristics;
    uint32_t size_of_stack_reserve;
    uint32_t size_of_stack_commit;
    uint32_t size_of_heap_reserve;
    uint32_t size_of_heap_commit;
    uint32_t loader_flags;
    uint32_t number_of_rva_and_sizes;

    //data directory
    struct data_directory dir[16];
} __attribute__((packed));

struct pe32p_optional {
    // standard_fields
    uint16_t magic;
    uint8_t major_linker_version;
    uint8_t minor_linker_version;
    uint32_t size_of_code;
    uint32_t size_of_initialized_data;
    uint32_t size_of_uninitialized_data;
    uint32_t address_of_entry_point;
    uint32_t base_of_code;

    // specific fields
    uint64_t image_base;
    uint32_t section_alignment;
    uint32_t file_alignment;
    uint16_t major_os_version;
    uint16_t minor_os_version;
    uint16_t major_image_version;
    uint16_t minor_image_version;
    uint16_t major_subsystem_version;
    uint16_t minor_subsystem_version;
    uint32_t win32_version_value;
    uint32_t size_of_image;
    uint32_t size_of_headers;
    uint32_t checksum;
    uint16_t subsystem;
    uint16_t dll_characteristics;
    uint64_t size_of_stack_reserve;
    uint64_t size_of_stack_commit;
    uint64_t size_of_heap_reserve;
    uint64_t size_of_heap_commit;
    uint32_t loader_flags;
    uint32_t number_of_rva_and_sizes;

    // data directory
    struct data_directory dir[16];
} __attribute__((packed));

struct section_table {
    uint8_t name[8];
    uint32_t virtual_size;
    uint32_t virtual_address;
    uint32_t size_of_raw_data;
    uint32_t pointer_to_raw_data;
    uint32_t pointer_to_relocations;
    uint32_t pointer_to_linenumbers;
    uint16_t number_of_relocations;
    uint16_t number_of_linenumbers;
    uint32_t characteristics;
};

struct import_descriptor {
    uint32_t original_first_thunk;
    uint32_t time_date_stamp;
    uint32_t forwarder_chain;
    uint32_t name;
    uint32_t first_thunk;
};

struct export_descriptor {
    uint32_t export_flags;
    uint32_t time_date_stamp;
    uint16_t major_version;
    uint16_t minor_version;
    uint32_t name_rva;
    uint32_t ordinal_base;
    uint32_t address_table_entries;
    uint32_t number_of_name_pointers;
    uint32_t export_address_table_rva;
    uint32_t name_pointer_rva;
    uint32_t ordinal_table_rva;
};

struct eat {
    uint32_t export_rva;
    uint32_t forwarder_rva;
};

struct pe {
    uint16_t mz_sig;
    uint32_t *pe_sig;
    struct coff_header *coff;
    union {
        struct pe32p_optional *pe32p;
        struct pe32_optional *pe32;
    } optional;
    struct section_table *section_table;
    uint16_t optional_magic;
};

struct section_table *get_section(struct section_table *table, uint16_t tables_count, const char *name);
uint8_t *load_section(uint8_t *source, uint32_t limit, uint8_t *destination, int prot);
uint8_t *load_sections(uint8_t *raw_file, struct pe *pe);

struct pe *parse(int fd, uint8_t **file, struct stat *sb, int image_type);

int load_dll(struct pe *pe, uint8_t *mapped_file, uint32_t *count);

#endif
