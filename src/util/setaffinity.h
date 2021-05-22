/************************************************
 * @addtogroup CITS
 * @{
 * @file  : setaffinity.h
 * @brief : 
 * @author: wangshaobo
 * @email : wangshaobo@nebula-link.com
 * @date  : 2019-05-10 10:43:51
************************************************/

//-----------------------------------------------
// Copyright (c) CITS
//-----------------------------------------------

#ifndef _SETAFFINITY_H_
#define _SETAFFINITY_H_

#ifdef __cplusplus
extern "C"
{
#endif

int thread_set_cpu(int index);

int process_set_cpu(int index);

#ifdef __cplusplus
}
#endif

#endif
