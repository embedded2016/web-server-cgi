#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

#include "cgi.h"
#include "pool/cgi_thread_pool.h"

static void* print(void *arg)
{
    printf("%d\n", (long)arg);
    return NULL;
}

static void* consumer_one(void *arg)
{
    cgi_thread_pool_t *pool = (cgi_thread_pool_t*)arg;
    long i;
    for(i = -1; i > -10; --i) {
        cgi_thread_pool_execute(pool, print, (void*)i);
    }
    return NULL;
}

static void* consumer_two(void *arg)
{
    cgi_thread_pool_t *pool = (cgi_thread_pool_t*)arg;
    long i;
    for(i = 1; i < 10; ++i) {
        cgi_thread_pool_execute(pool, print, (void*)i);
    }
    return NULL;
}

int main()
{
    cgi_thread_pool_t *pool = cgi_thread_pool_create();
    cgi_thread_pool_start(pool);
    pthread_t tido, tidt;
    if(pthread_create(&tido, NULL, consumer_one, (void*)pool)) {
        perror("pthread_create: consumer_one");
    }
    if(pthread_create(&tidt, NULL, consumer_two, (void*)pool)) {
        perror("pthread_create: consumer_two");
    }
    sleep(3);
    cgi_thread_pool_destroy(pool);
    return 0;
}
