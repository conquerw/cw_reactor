/************************************************
 * @addtogroup CITS
 * @{
 * @file  : event.h
 * @brief : 
 * @author: wangshaobo
 * @email : wangshaobo@nebula-link.com
 * @date  : 2019-05-10 10:43:51
************************************************/

//-----------------------------------------------
// Copyright (c) CITS
//-----------------------------------------------

#ifndef _EVENT_H_
#define _EVENT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <map.h>

typedef int (*event_cb)(int, int, void *);

typedef struct
{
	int fd;
	int events;
	void *para;
	event_cb send_cb;
	event_cb recv_cb;
	event_cb error_cb;
	char *send_buffer;
	int send_buffer_len;
	map_node node;
} event_t;

void event_set(event_t *event, int fd, event_cb send_cb, event_cb recv_cb, event_cb error_cb, void *para);

#ifdef __cplusplus
}
#endif

#endif
