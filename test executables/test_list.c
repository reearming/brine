#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct item {
    int32_t value;
    struct item *next;
};


void add_an_item(struct item *list, int32_t value) {
    struct item *new_item = malloc(sizeof(struct item));
    new_item->value = value;

    struct item *current_item = list;
    for (size_t i = 0; current_item->next != NULL; i++) {
        current_item = current_item->next;
    }
    current_item->next = new_item;
}

void remove_an_item(struct item *list, size_t index) {
    struct item *current_item = list;
    for (size_t i = 0; i < index; i++) {
        current_item = current_item->next;
    }
    struct item *item_to_remove = current_item->next;
    current_item->next = current_item->next->next;
    free(item_to_remove);
}

int32_t read_an_item(struct item *list, size_t index) {
    struct item *current_item = list;
    for (size_t i = 0; i < index; i++) {
        current_item = current_item->next;
    }
    return current_item->value;
}

int main() {
    struct item *list = malloc(sizeof(struct item));
    int32_t value;
    printf("enter a signed 32 bit integer\n");
    scanf("%d", &value);
    list->value = value;

    for (int i = 0; i < 4; i++) {
        int32_t new_value;
        printf("enter a signed 32 bit integer\n");
        scanf("%d", &new_value);
        add_an_item(list, new_value);
    }
    while (1) {
        int action;
        printf("what do you wanna do? 0 - quit 1 - add 2 - remove 3 - read\n");
        scanf("%d", &action);
        if (action == 1) {
            size_t item;
            printf("item to add?\n");
            scanf("%lld", &item);
            add_an_item(list, item);
        }
        else if (action == 2) {
            size_t item;
            printf("item to remove?\n");
            scanf("%lld", &item);
            remove_an_item(list, item);
        }
        else if (action == 3) {
            size_t item;
            printf("item to read?\n");
            scanf("%lld", &item);
            printf("%d\n", read_an_item(list, item));
        }
        else if (action == 0) {
            break;
        }
        else {
            continue;
        }
    }

    return 0;
}

