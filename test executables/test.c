#include <stdio.h>

int main() {
    int value;
    printf("enter an integer value (for example %d, %d)\n", 12, 22);
    scanf("%d", &value);
    printf("value + 2: %d\n", value + 2);
    
    printf("string: %s\n", "the quick brown fox jumps over the lazy dog");

    printf("no va_list integer overflow: %d, %d, %d, %d\n", 1, 2, 3, 4);
    printf("integer overflow by 3: %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
    printf("no va_list float overflow: %f, %f, %f, %f, %f, %f, %f, %f\n", 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f);
    printf("float overflow by 3: %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.1f, 1.2f);
    printf("va_list overflow by 3: %d, %f, %f, %d, %f, %d, %f, %f, %f, %f, %d\n", 1, 0.2f, 0.3f, 4, 0.5f, 6, 0.7f, 0.8f, 0.9f, 1.1f, 12);

    printf("f: %f\n", 12345.12345f);
    printf("Lf: %Lf, d: %lld\n", 12345.12345L, sizeof(long double));

    printf("lld: %lld\n", 1234556LL);
    return 0;
}
