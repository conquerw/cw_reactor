/************************************************
 * @addtogroup CITS
 * @{
 * @file  : event.c
 * @brief : 
 * @author: wangshaobo
 * @email : wangshaobo@nebula-link.com
 * @date  : 2019-05-10 10:43:51
************************************************/

//-----------------------------------------------
// Copyright (c) CITS
//-----------------------------------------------

#include <stdlib.h>
#include <event.h>

void event_set(event_t *event, int fd, event_cb send_cb, event_cb recv_cb, event_cb error_cb, void *para)
{
	event->fd = fd;
	event->events = 0;
	event->para = para;
	event->send_cb = send_cb;
	event->recv_cb = recv_cb;
	event->error_cb = error_cb;	
	event->send_buffer = NULL;
	event->send_buffer_len = 0;
}
