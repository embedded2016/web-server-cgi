#include <stdio.h>

#include "cgi.h"
#include "utils/cgi_task_queue.h"

static void* print(void *arg)
{
    printf("%ld\n", (long) arg);
    return NULL;
}

int main()
{
    cgi_task_queue_t *head = NULL;
    for (long i = 0; i < 10; ++i)
        cgi_task_queue_push(&head, print, (void *) i);
    cgi_task_queue_t *tmp = NULL;
    for (;;) {
        tmp = cgi_task_queue_pop(&head);
        if (tmp == NULL)
            break;
        tmp->callback(tmp->arg);
    }
    return 0;
}
