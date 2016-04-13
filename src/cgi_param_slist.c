#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "factory/cgi_factory.h"
#include "utils/cgi_slist.h"
#include "utils/cgi_param_slist.h"

cgi_pslist_t *cgi_pslist_create(char *key,char *value)
{
    cgi_pslist_t *param = cgi_factory_create(PARAM_SLIST);

    param->key = key;
    param->value = value;
    CGI_SLIST_INIT(param,linker);

    return param;
}

int cgi_pslist_is_empty(cgi_pslist_t *head)
{
    return CGI_SLIST_IS_EMPTY(head);
}

void cgi_pslist_insert_head(cgi_pslist_t **head_ptr,cgi_pslist_t *elem)
{
    if (*head_ptr != elem)
        CGI_SLIST_INSERT_HEAD(*head_ptr,elem,linker);
}

void cgi_pslist_remove_head(cgi_pslist_t **head_ptr)
{
    if (!CGI_SLIST_IS_EMPTY(*head_ptr)) {
        cgi_pslist_t *head = *head_ptr;
        CGI_SLIST_REMOVE_HEAD(*head_ptr,linker);
        cgi_pslist_delete(head);
    }
}

char* cgi_pslist_find(cgi_pslist_t *head,char *key)
{
    cgi_pslist_t *tmp = NULL;
    CGI_SLIST_FOREACH(tmp,head,linker) {
        if (strcmp(tmp->key,key) == 0)
            break;
    }
    return tmp != NULL ? tmp->value : NULL;
}

void cgi_pslist_delete(cgi_pslist_t *elem)
{
    cgi_factory_destroy(elem,PARAM_SLIST);
}

void cgi_pslist_destroy(cgi_pslist_t **head_ptr)
{
    if (!CGI_SLIST_IS_EMPTY(*head_ptr)) {
        cgi_pslist_t *head = *head_ptr;
        cgi_pslist_destroy(&CGI_SLIST_NEXT(head,linker));
        cgi_pslist_delete(head);
    }
}
