/************************************************
 * @addtogroup CITS
 * @{
 * @file  : work.c
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
#include <unistd.h>
#include <setaffinity.h>
#include <queue.h>
#include <thread.h>
#include <work.h>

static pthread_t *pthread = NULL;
// static int cpu_index = 0;

static void *run(void *para)
{
	if(!para)
		return NULL;
	
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	
	// thread_set_cpu(cpu_index++);
	
	queue_t *queue = (queue_t *)para;
	
	while(1)
	{
		struct slist_node *node = dequeue(queue);
		if(node)
		{
			work_t *work = slist_entry(node, work_t, node);
			if(work)
			{
				if(work->run)
					work->run(work->para);
				free(work);
			}
		}
	}
	
	return NULL;
}

int work_init(reactors_t *reactors)
{
	if(!reactors || pthread)
		return -1;
	
	pthread = (pthread_t *)calloc(reactors->reactor_len, sizeof(pthread_t));
	for(int i = 0; i < reactors->reactor_len; i++)
		create_thread2(pthread + i, run, (reactors->reactor + i)->queue);
	
	return 1;
}

int work_exit(reactors_t *reactors)
{
	if(!reactors || !pthread)
		return -1;
	
	for(int i = 0; i < reactors->reactor_len; i++)
		cancel_thread(*(pthread + i));
	free(pthread);
	
	return 1;
}
