#include <stdint.h>
#include <stdio.h>

extern uint32_t fourfourfour();

int main() {
    printf("it works\n");
    int a = 40;
    int b = 2;
    a += b;
    fourfourfour();
    for (;;) {}

    return 0;
}
