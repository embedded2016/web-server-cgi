#include <stdio.h>
#include <stdlib.h>

#include "utils/cgi_slist.h"

typedef struct single_list slist_t;

struct single_list {
    int data;
    CGI_SLIST_ENTRY(slist_t) linker;
};

static slist_t* slist_create(int data)
{
    slist_t *entry = (slist_t*)malloc(sizeof(slist_t));
    entry->data = data;
    CGI_SLIST_INIT(entry,linker);
    return entry;
}

static int slist_is_empty(slist_t *head)
{
    return CGI_SLIST_IS_EMPTY(head);
}

static void slist_insert_head(slist_t **head_ptr,int data)
{
    slist_t *entry = slist_create(data);
    if(slist_is_empty(*head_ptr)) {
        *head_ptr = entry;
    } else {
        CGI_SLIST_INSERT_HEAD(*head_ptr,entry,linker);
    }
}

static void slist_insert_after(slist_t **head_ptr,slist_t *elem)
{
    if(slist_is_empty(*head_ptr)) {
        *head_ptr = elem;
    } else {
        CGI_SLIST_INSERT_AFTER(*head_ptr,elem,linker);
    }
}

static void slist_print(slist_t *head)
{
    slist_t *var = NULL;
    CGI_SLIST_FOREACH(var,head,linker) {
        printf("%d ",var->data);
    }
    printf("\n");
}

static void slist_destroy(slist_t **head_ptr)
{
    if(!slist_is_empty(*head_ptr)) {
        slist_t *head = *head_ptr;
        slist_destroy(&CGI_SLIST_NEXT(*head_ptr,linker));
        free(head);
    }
}

int main()
{
    slist_t *slist = NULL;
    printf("Is empty?\t%d\n",slist_is_empty(slist));

    slist_insert_head(&slist,9);
    slist_insert_after(&slist,slist_create(8));
    slist_print(slist);

    slist_t *tmp = slist;
    CGI_SLIST_REMOVE_HEAD(slist,linker);
    slist_print(slist);

    slist_insert_after(&slist,tmp);
    slist_print(slist);

    CGI_SLIST_REMOVE(slist_t,slist,tmp,linker);
    slist_print(slist);

    slist_insert_after(&slist,tmp);
    slist_print(slist);

    slist_destroy(&slist);
    return 0;
}
