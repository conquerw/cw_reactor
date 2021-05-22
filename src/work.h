/************************************************
 * @addtogroup CITS
 * @{
 * @file  : work.h
 * @brief : 
 * @author: wangshaobo
 * @email : wangshaobo@nebula-link.com
 * @date  : 2019-05-10 10:43:51
************************************************/

//-----------------------------------------------
// Copyright (c) CITS
//-----------------------------------------------

#ifndef _WORK_H_
#define _WORK_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <slist.h>
#include <reactors.h>

typedef struct
{
	void *para;
	int (*run)(void *para);
	struct slist_node node;
} work_t;

int work_init(reactors_t *reactors);

int work_exit(reactors_t *reactors);

#ifdef __cplusplus
}
#endif

#endif
