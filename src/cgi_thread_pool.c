#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>

#include "cgi.h"
#include "factory/cgi_factory.h"
#include "pool/cgi_thread_pool.h"
#include "utils/cgi_task_queue.h"

static void *cgi_thread_pool_run(void *arg);

cgi_thread_pool_t *cgi_thread_pool_create()
{
    cgi_thread_pool_t *pool = (cgi_thread_pool_t *)
        cgi_factory_create(THREAD_POOL);
    pool->head = NULL;
    pool->sflag = 1;
    pool->size = CGI_THREAD_POOL_SIZE;
    pthread_mutex_init(&pool->mutex, NULL);
    sem_init(&pool->semaphore, 0, 0);
    return pool;
}

void cgi_thread_pool_start(cgi_thread_pool_t *pool)
{
    if (pool->sflag) {
        pool->sflag = 0;
        for (uint32_t i = 0; i < pool->size; ++i)
            pthread_create(pool->tids + i, NULL, cgi_thread_pool_run, pool);
    }
}

void *cgi_thread_pool_run(void *arg)
{
    cgi_thread_pool_t *pool = (cgi_thread_pool_t *) arg;
    while (!pool->sflag) {
        cgi_task_queue_t *node = cgi_task_queue_take(pool);
        node->callback(node->arg);
        cgi_task_queue_delete(node);
    }
}

void cgi_thread_pool_execute(cgi_thread_pool_t *pool,
                             void *(*callback)(void *), void *arg)
{
    cgi_task_queue_put(pool, callback, arg);
}

void cgi_thread_pool_stop(cgi_thread_pool_t *pool)
{
    pool->sflag = 1;
}

void cgi_thread_pool_destroy(cgi_thread_pool_t *pool)
{
    cgi_thread_pool_stop(pool);
    sem_destroy(&pool->semaphore);
    pthread_mutex_destroy(&pool->mutex);
    cgi_task_queue_destroy(&pool->head);
    cgi_factory_destroy(pool, THREAD_POOL);
}
