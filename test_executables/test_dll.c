#include <stdint.h>

extern uint32_t fourfourfour();

int main() {
    int a = 40;
    int b = 2;
    a += b;
    fourfourfour();
    for (;;) {}

    return 0;
}
