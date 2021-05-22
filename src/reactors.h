/************************************************
 * @addtogroup CITS
 * @{
 * @file  : reactors.h
 * @brief : 
 * @author: wangshaobo
 * @email : wangshaobo@nebula-link.com
 * @date  : 2019-05-10 10:43:51
************************************************/

//-----------------------------------------------
// Copyright (c) CITS
//-----------------------------------------------

#ifndef _REACTORS_H_
#define _REACTORS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <reactor.h>

typedef struct
{
	reactor_t *reactor;
	int reactor_len;
} reactors_t;

int reactors_init(reactors_t *reactors);

int reactors_exit(reactors_t *reactors);

int reactors_run(reactors_t *reactors);

int reactors_stop(reactors_t *reactors);

#ifdef __cplusplus
}
#endif

#endif
