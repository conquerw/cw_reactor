/************************************************
 * @addtogroup CITS
 * @{
 * @file  : queue.h
 * @brief : 
 * @author: wangshaobo
 * @email : wangshaobo@nebula-link.com
 * @date  : 2019-05-10 10:43:51
************************************************/

//-----------------------------------------------
// Copyright (c) CITS
//-----------------------------------------------

#ifndef _QUEUE_H_
#define _QUEUE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <pthread.h>
#include <slist.h>

typedef struct
{
	int max;
	int enqueue_len;
	int dequeue_len;
	pthread_mutex_t enqueue_mutex;
	pthread_mutex_t dequeue_mutex;
	pthread_cond_t enqueue_cond;
	pthread_cond_t dequeue_cond;
	struct slist_head enqueue_node;
	struct slist_head dequeue_node;
} queue_t;

static inline int queue_init(queue_t *queue, int max)
{
    if(!queue)
    {
        printf("params error\n");
        return -1;
    }
	
	queue->max = max;
	queue->enqueue_len = 0;
    queue->dequeue_len = 0;
    pthread_mutex_init(&queue->enqueue_mutex, NULL);
	pthread_mutex_init(&queue->dequeue_mutex, NULL);
	pthread_cond_init(&queue->enqueue_cond, NULL);
	pthread_cond_init(&queue->dequeue_cond, NULL);
	INIT_SLIST_HEAD(&queue->enqueue_node);
	INIT_SLIST_HEAD(&queue->dequeue_node);
	
    return 1;
}

static inline int queue_exit(queue_t *queue)
{
    if(!queue)
    {
        printf("params error\n");
        return -1;
    }
	
	INIT_SLIST_HEAD(&queue->dequeue_node);
	INIT_SLIST_HEAD(&queue->enqueue_node);
	pthread_cond_destroy(&queue->dequeue_cond);
	pthread_cond_destroy(&queue->enqueue_cond);
    pthread_mutex_destroy(&queue->dequeue_mutex);
	pthread_mutex_destroy(&queue->enqueue_mutex);
	queue->dequeue_len = 0;
	queue->enqueue_len = 0;
	queue->max = 0;
	
    return 1;
}

static inline int enqueue(queue_t *queue, struct slist_node *node)
{
	if(!queue || !node)
    {
        printf("params error\n");
        return -1;
    }
	
    pthread_mutex_lock(&queue->enqueue_mutex);
	while(queue->enqueue_len > queue->max - 1)
		pthread_cond_wait(&queue->enqueue_cond, &queue->enqueue_mutex);
	queue->enqueue_len++;
	slist_add_tail(node, &queue->enqueue_node);
	pthread_mutex_unlock(&queue->enqueue_mutex);
	pthread_cond_signal(&queue->dequeue_cond);
    return 1;
}

static inline int swap_list(queue_t *queue)
{
    if(!queue)
    {
        printf("params error\n");
        return -1;
    }
	
	pthread_mutex_lock(&queue->enqueue_mutex);
	while(queue->enqueue_len == 0)
		pthread_cond_wait(&queue->dequeue_cond, &queue->enqueue_mutex);
	queue->dequeue_len = queue->enqueue_len;
	slist_move(&queue->dequeue_node, &queue->enqueue_node);
	queue->enqueue_len = 0;
	pthread_mutex_unlock(&queue->enqueue_mutex);
	pthread_cond_signal(&queue->enqueue_cond);
	return queue->dequeue_len;
}

static inline struct slist_node *dequeue(queue_t *queue)
{
    if(!queue)
    {
        printf("params error\n");
        return NULL;
    }
	
	struct slist_node *node = NULL;
    pthread_mutex_lock(&queue->dequeue_mutex);
	if(!slist_empty(&queue->dequeue_node) || swap_list(queue) > 0)
	{
		queue->dequeue_len--;
		node = queue->dequeue_node.first.next;
		slist_del_head(&queue->dequeue_node);
	}
	pthread_mutex_unlock(&queue->dequeue_mutex);
	return node;
}

static inline int queue_len(queue_t *queue)
{
    if(!queue)
    {
        printf("params error\n");
        return -1;
    }
	
	return queue->enqueue_len + queue->dequeue_len;
}

#ifdef __cplusplus
}
#endif

#endif
