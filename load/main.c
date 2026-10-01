#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include "loader.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("no arguments provided\n");
        return 1;
    } 
    int fd = open(argv[1], O_RDONLY);
    try_PE(fd);

    return 0;
}
