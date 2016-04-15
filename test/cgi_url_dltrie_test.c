#include <stdio.h>
#include <stdlib.h>

#include "cgi.h"
#include "utils/cgi_url_dltrie.h"

static void function_handler(cgi_http_connection_t *connection)
{
    printf("callback\n");
}

static void travel(cgi_url_dltrie_t *url_trie)
{
    if (url_trie != NULL) {
        printf("%s\n",url_trie->key);
        travel(url_trie->linker.child);
        travel(url_trie->linker.sibling);
    }
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr,"No enough parameter.\n");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    FILE *fp = fopen(argv[1],"r");

    cgi_url_dltrie_t *url_trie = NULL;
    while (!feof(fp)) {
        fscanf(fp,"%s",buffer);
        cgi_url_dltrie_insert(&url_trie, buffer, function_handler, NULL);
        cgi_handler_t handler = cgi_url_dltrie_find(url_trie, buffer);
        handler(NULL);
    }

    travel(url_trie);

    cgi_url_dltrie_t *default_root = cgi_url_dltrie_default_root();
    cgi_handler_t handler = cgi_url_dltrie_find(default_root, "/");
    handler(NULL);
    handler = cgi_url_dltrie_find(default_root, "/error.html");
    handler(NULL);

    return 0;
}
