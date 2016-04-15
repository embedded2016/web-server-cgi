#include <pthread.h>
#include <semaphore.h>

#include "cgi.h"
#include "factory/cgi_factory.h"
#include "utils/cgi_task_queue.h"

cgi_task_queue_t *cgi_task_queue_create(void *(*callback)(void *), void *arg)
{
    cgi_task_queue_t *node = cgi_factory_create(TASK_QUEUE);
    node->callback = callback;
    node->arg = arg;
    node->linker.next = NULL;
    return node;
}

cgi_task_queue_t *cgi_task_queue_take(cgi_thread_pool_t *pool)
{
    cgi_task_queue_t *head = NULL;
    sem_wait(&pool->semaphore);
    pthread_mutex_lock(&pool->mutex);
    head = cgi_task_queue_pop(&pool->head);
    pthread_mutex_unlock(&pool->mutex);
    return head;
}

void cgi_task_queue_put(cgi_thread_pool_t *pool,
                        void *(*callback)(void *), void *arg)
{
    pthread_mutex_lock(&pool->mutex);
    cgi_task_queue_push(&pool->head, callback, arg);
    pthread_mutex_unlock(&pool->mutex);
    sem_post(&pool->semaphore);
}

void cgi_task_queue_push(cgi_task_queue_t **head_ptr,
                         void *(*callback)(void *), void *arg)
{
    cgi_task_queue_t *head = *head_ptr;
    if (head != NULL)
        cgi_task_queue_push(&head->linker.next, callback, arg);
    else
        *head_ptr = cgi_task_queue_create(callback, arg);
}

cgi_task_queue_t *cgi_task_queue_pop(cgi_task_queue_t **head_ptr)
{
    cgi_task_queue_t *head = *head_ptr;
    if (head != NULL) {
        *head_ptr = head->linker.next;
        head->linker.next = NULL;
    }
    return head;
}

void cgi_task_queue_delete(cgi_task_queue_t *node)
{
    cgi_factory_destroy(node, TASK_QUEUE);
}

void cgi_task_queue_destroy(cgi_task_queue_t **head_ptr)
{
    cgi_task_queue_t *head = *head_ptr;
    if (head != NULL) {
        cgi_task_queue_destroy(&head->linker.next);
        cgi_task_queue_delete(head);
    }
}
