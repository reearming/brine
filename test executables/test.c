#include <stdio.h>

int main() {
    int value;
    printf("enter an integer value (for example %d, %d)\n", 12, 22);
    scanf("%d", &value);
    printf("value + 2: %d\n", value + 2);

    printf("no va_list overflow: %d, %d, %d, %d\n", 1, 2, 3, 4);
    printf("overflow by 1: %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5);
    printf("overflow by 2: %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6);
    printf("overflow by 3: %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
    printf("string: %s\n", "the quick brown fox jumps over the lazy dog");
    return 0;
}
