/************************************************
 * @addtogroup CITS
 * @{
 * @file  : reactors.c
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
#include <thread.h>
#include <reactors.h>

static pthread_t *pthread = NULL;

int reactors_init(reactors_t *reactors)
{
	if(!reactors)
		return -1;
	
	reactors->reactor_len = sysconf(_SC_NPROCESSORS_CONF) / 2;
	reactors->reactor = (reactor_t *)calloc(reactors->reactor_len, sizeof(reactor_t));
	for(int i = 0; i < reactors->reactor_len; i++)
		reactor_init(reactors->reactor + i);
	
	return 1;
}

int reactors_exit(reactors_t *reactors)
{
	if(!reactors)
		return -1;
	
	for(int i = 0; i < reactors->reactor_len; i++)
		reactor_exit(reactors->reactor + i);
	free(reactors->reactor);
	
	return 1;
}

int reactors_run(reactors_t *reactors)
{
	if(!reactors || pthread)
		return -1;
	
	pthread = (pthread_t *)calloc(reactors->reactor_len, sizeof(pthread_t));
	for(int i = 0; i < reactors->reactor_len; i++)
		create_thread2(pthread + i, reactor_run, reactors->reactor + i);
	
	return 1;
}

int reactors_stop(reactors_t *reactors)
{
	if(!reactors || !pthread)
		return -1;
	
	for(int i = 0; i < reactors->reactor_len; i++)
		cancel_thread(*(pthread + i));
	free(pthread);
	
	return 1;
}
