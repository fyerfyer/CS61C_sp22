#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
    /* TODO: Implement ll_has_cycle */
    if (head == NULL || head->next == NULL) return 0;

    node *slowptr = head;
    node *fastptr = head;

    while (fastptr != NULL && fastptr->next != NULL) {
        slowptr = slowptr->next;
        fastptr = fastptr->next->next;
        if (fastptr == slowptr) return 1;
    }

    return 0;
}
