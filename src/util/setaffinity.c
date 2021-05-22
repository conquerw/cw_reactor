/************************************************
 * @addtogroup CITS
 * @{
 * @file  : setaffinity.c
 * @brief : 
 * @author: wangshaobo
 * @email : wangshaobo@nebula-link.com
 * @date  : 2019-05-10 10:43:51
************************************************/

//-----------------------------------------------
// Copyright (c) CITS
//-----------------------------------------------

#include <stdio.h>
#include <unistd.h>
#define __USE_GNU
#include <sched.h>
#include <pthread.h>
#include <setaffinity.h>

int thread_set_cpu(int index)
{
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(index, &mask);
	
	printf("thread %lu, index = %d\n", pthread_self(), index);
	int ret = pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask);
	if(ret < 0)
		return -1;
	return 1;
}

int process_set_cpu(int index)
{
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(index, &mask);
	
	int ret = sched_setaffinity(0, sizeof(mask), &mask);
	if(ret < 0)
		return -1;
	return 1;
}
