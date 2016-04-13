#ifndef CGI_DLTRIE_H
#define CGI_DLTRIE_H

#define CGI_DLTRIE_ENTRY(type)	\
    struct {                    \
        type *sibling;          \
	type *child;            \
    }

#define CGI_DLTRIE_FIRST(head)	(head)
#define CGI_DLTRIE_SIBLING(elem,field)	((elem)->field.sibling)
#define CGI_DLTRIE_CHILD(elem,field)	((elem)->field.child)
#define CGI_DLTRIE_END(head)	NULL

#define CGI_DLTRIE_IS_EMPTY(head) \
    (CGI_DLTRIE_FIRST(head) == CGI_DLTRIE_END(head))

#define CGI_DLTRIE_INIT(elem, field) \
    ((elem)->field.sibling = (elem)->field.child = NULL)

#define CGI_DLTRIE_INSERT_AFTER(elemo,elemt,field,member)	\
    do {                                                        \
        (elemt)->field.member = (elemo)->field.member;          \
        (elemo)->field.member = (elemt);                        \
    } while (0)

#define CGI_DLTRIE_INSERT_AFTER_AS_SIBLING(elemo,elemt,field)   \
    CGI_DLTRIE_INSERT_AFTER(elemo, elemt, field, sibling)

#define CGI_DLTRIE_INSERT_AFTER_AS_CHILD(elemo,elemt,field)     \
    CGI_DLTRIE_INSERT_AFTER(elemo, elemt, field, child)

#endif
