/***********************************************
 * @addtogroup CITS
 * @{
 * @file  : thread.h
 * @brief : 
 * @author: wangshaobo
 * @email : wangshaobo@nebula-link.com
 * @date  : 2019-05-10 10:43:51
***********************************************/

//--------------------------------------------------
// Copyright (c) CITS
//--------------------------------------------------

#ifndef _THREAD_H_
#define _THREAD_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

static inline int create_thread(void *(*func)(void *), void *para)
{
	pthread_t pthread;
	
	int ret = pthread_create(&pthread, NULL, func, para);
	if(ret == 0)
	{
		printf("create_thread success!\n");
		usleep(50 * 1000);
	}
	else
	{
		printf("create_thread fail!\n");
		return -1;
	}
	
	pthread_detach(pthread);
	
	return 1;
}

static inline int create_thread2(pthread_t *pthread, void *(*func)(void *), void *para)
{
	int ret = pthread_create(pthread, NULL, func, para);
	if(ret == 0)
	{
		printf("create_thread2 success!\n");
		usleep(50 * 1000);
	}
	else
	{
		printf("create_thread2 fail!\n");
		return -1;
	}
	
	pthread_detach(*pthread);
	
	return 1;
}

static inline void cancel_thread(pthread_t pthread)
{
	pthread_cancel(pthread);
}

#ifdef __cplusplus
}
#endif

#endif
