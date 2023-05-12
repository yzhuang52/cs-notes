#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
    node *runner = head;
    node *walker = head;
    while (runner != NULL && runner->next != NULL) {
        runner = runner->next->next;
        walker = walker->next;
        if (runner == walker) {
            return 1;
        }
    }
    return 0;
}