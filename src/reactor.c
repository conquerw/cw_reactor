/************************************************
 * @addtogroup CITS
 * @{
 * @file  : reactor.c
 * @brief : 
 * @author: wangshaobo
 * @email : wangshaobo@nebula-link.com
 * @date  : 2019-05-10 10:43:51
************************************************/

//-----------------------------------------------
// Copyright (c) CITS
//-----------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <setaffinity.h>
#include <thread.h>
#include <reactor.h>

// static int cpu_index = 0;

int reactor_init(reactor_t *reactor)
{
	reactor->epoll_fd = epoll_create(1);
	reactor->epoll_event = (struct epoll_event *)calloc(EPOLL_EVENT_LEN, sizeof(struct epoll_event));
	reactor->epoll_event_len = EPOLL_EVENT_LEN;
	reactor->root = RB_ROOT;
	reactor->queue = (queue_t *)calloc(1, sizeof(queue_t));
	queue_init(reactor->queue, 1024);
	
	return 1;
}

static void reactor_event_free(map_root *root)
{
	event_t *link = NULL, *next = NULL;
	map_for_each_entry_safe(link, next, root, node)
	{
		map_remove(root, link->node.key);
		free(link);
	}
}

int reactor_exit(reactor_t *reactor)
{
	queue_exit(reactor->queue);
	free(reactor->queue);
	reactor_event_free(&reactor->root);
	free(reactor->epoll_event);
	close(reactor->epoll_fd);
	
	return 1;
}

int reactor_add_event(reactor_t *reactor, event_t *event, int events)
{
    struct epoll_event ev;
	memset(&ev, 0x00, sizeof(ev));
    ev.events = event->events = events;
    ev.data.ptr = event;
	
    if(epoll_ctl(reactor->epoll_fd, EPOLL_CTL_ADD, event->fd, &ev) < 0)
    {
        printf("event add failed [fd=%d], events[%d]\n", event->fd, events);
        return -1;
    }
	
	return 1;
}

int reactor_delete_event(reactor_t *reactor, event_t *event)
{
    struct epoll_event ev;
	memset(&ev, 0x00, sizeof(ev));
    ev.data.ptr = event;
	
    if(epoll_ctl(reactor->epoll_fd, EPOLL_CTL_DEL, event->fd, &ev) < 0)
    {
        printf("event add failed [fd=%d]\n", event->fd);
        return -1;
    }
	
	return 1;
}

int reactor_update_event(reactor_t *reactor, event_t *event, int events)
{
    struct epoll_event ev;
	memset(&ev, 0x00, sizeof(ev));
    ev.events = event->events = events;
    ev.data.ptr = event;
	
    if(epoll_ctl(reactor->epoll_fd, EPOLL_CTL_MOD, event->fd, &ev) < 0)
    {
        printf("event add failed [fd=%d], events[%d]\n", event->fd, events);
        return -1;
    }
	
	return 1;
}

void *reactor_run(void *para)
{
	if(!para)
		return NULL;
	
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	
	// thread_set_cpu(cpu_index++);
	
	reactor_t *reactor = (reactor_t *)para;
	// int timeout = 0;
	
	while(1)
	{
		memset(reactor->epoll_event, 0x00, sizeof(struct epoll_event) * reactor->epoll_event_len);
		// int nfds = epoll_wait(reactor->epoll_fd, reactor->epoll_event, reactor->epoll_event_len, timeout);
		int nfds = epoll_wait(reactor->epoll_fd, reactor->epoll_event, reactor->epoll_event_len, -1);
		if(nfds == -1)
		{
			printf("epoll_wait error, exit\n");
			// timeout = -1;
            // sched_yield();
			continue;
		}
		
		// timeout = 0;
		
		for(int i = 0; i < nfds; i++)
		{
			event_t *event = (event_t *)reactor->epoll_event[i].data.ptr;
			
			if((reactor->epoll_event[i].events & EPOLLIN) && (event->events & EPOLLIN))
				if(event->recv_cb)
					event->recv_cb(event->fd, reactor->epoll_event[i].events, event->para);
			
			if((reactor->epoll_event[i].events & EPOLLOUT) && (event->events & EPOLLOUT))
				if(event->error_cb)
					event->error_cb(event->fd, reactor->epoll_event[i].events, event->para);
		}
		
		if(reactor->epoll_event_len == nfds)
		{
			
		}
	}
	
	return NULL;
}
