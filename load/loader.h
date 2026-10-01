#ifndef LOADER_H
#define LOADER_H

#include <stdint.h>
#include <stdio.h>

#define MZ_SIGNATURE 0x5A4D
#define PE_HEADER_OFFSET 0x3C
#define IMAGE_FILE_MACHINE_AMD64 0x8664
#define IMAGE_FILE_MACHINE_I386 0x14c
#define IMAGE_FILE_EXECUTABLE_IMAGE 0x0002 
#define IMAGE_FILE_DLL 0x2000
#define PE32 0x10b
#define PE32P 0x20b

int try_PE(int fd);

struct COFF {
    uint16_t machine;
    uint16_t number_of_sections;
    uint32_t time_date_stamp;
    uint32_t pointer_to_symbol_table;
    uint32_t number_of_symbols;
    uint16_t size_of_optional_header;
    uint16_t characteristics;
};

#endif
