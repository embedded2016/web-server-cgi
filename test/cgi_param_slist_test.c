#include <stdio.h>

#include "utils/cgi_slist.h"
#include "utils/cgi_param_slist.h"

static void print(cgi_pslist_t *head)
{
    cgi_pslist_t *tmp = NULL;
    CGI_SLIST_FOREACH(tmp,head,linker) {
        printf("key:\t%s\t\tvalue:\t%s\n", tmp->key, tmp->value);
    }
}

int main()
{
    cgi_pslist_t *head = cgi_pslist_create("Hello", "Hi");
    print(head);
    printf("\n");

    cgi_pslist_insert_head(&head, cgi_pslist_create("A", "B"));
    print(head);
    printf("\n");

    cgi_pslist_insert_head(&head, cgi_pslist_create("C", "D"));
    print(head);
    printf("\n");

    cgi_pslist_insert_head(&head, cgi_pslist_create("E", "F"));
    print(head);
    printf("\n");

    cgi_pslist_insert_head(&head, cgi_pslist_create("G", "H"));
    print(head);
    printf("\n");

    printf("find:\t%s\t\t%s\n\n", "C", cgi_pslist_find(head, "C"));

    while (!cgi_pslist_is_empty(head)) {
        cgi_pslist_remove_head(&head);
        print(head);
        putchar('\n');
    }

    cgi_pslist_destroy(&head);
    return 0;
}
