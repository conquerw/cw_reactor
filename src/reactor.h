/************************************************
 * @addtogroup CITS
 * @{
 * @file  : reactor.h
 * @brief : 
 * @author: wangshaobo
 * @email : wangshaobo@nebula-link.com
 * @date  : 2019-05-10 10:43:51
************************************************/

//-----------------------------------------------
// Copyright (c) CITS
//-----------------------------------------------

#ifndef _REACTOR_H_
#define _REACTOR_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <pthread.h>
#include <queue.h>
#include <event.h>

#define EPOLL_EVENT_LEN 1024

typedef struct
{
	int socket_fd;
	int epoll_fd;
	struct epoll_event *epoll_event;
	int epoll_event_len;
	map_root root;
	queue_t *queue;
} reactor_t;

int reactor_init(reactor_t *reactor);

int reactor_exit(reactor_t *reactor);

int reactor_add_event(reactor_t *reactor, event_t *event, int events);

int reactor_delete_event(reactor_t *reactor, event_t *event);

int reactor_update_event(reactor_t *reactor, event_t *event, int events);

void *reactor_run(void *para);

#ifdef __cplusplus
}
#endif

#endif
