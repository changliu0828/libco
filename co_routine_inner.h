/*
* Tencent is pleased to support the open source community by making Libco available.

* Copyright (C) 2014 THL A29 Limited, a Tencent company. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License"); 
* you may not use this file except in compliance with the License. 
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, 
* software distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
* See the License for the specific language governing permissions and 
* limitations under the License.
*/


#ifndef __CO_ROUTINE_INNER_H__

#include "co_routine.h"
#include "coctx.h"
struct stCoRoutineEnv_t;
struct stCoSpec_t
{
	void *value;
};

struct stStackMem_t                                 //单个共享栈                             ------   
{                                                   //                                  |      |
	stCoRoutine_t* occupy_co;                       //指向当前占用共享栈的协程          | used | 
	int stack_size;                                 //空闲大小                          |------|  <- stack_bp    
	char* stack_bp; //stack_buffer + stack_size     //base pointer                      | free |    
	char* stack_buffer;                             //栈地址                            |      |
};                                                  //                                   ------   <- stack_buffer

struct stShareStack_t                               //共享栈
{
	unsigned int alloc_idx;                         //共享栈数组索引 TODO
	int stack_size;                                 //单个共享栈大小
	int count;                                      //共享栈个数
	stStackMem_t** stack_array;                     //共享栈数组
};



struct stCoRoutine_t
{
	stCoRoutineEnv_t *env;              //指向协程运行环境，各协程共享
	pfn_co_routine_t pfn;               //指向执行函数
	void *arg;                          //执行函数参数
	coctx_t ctx;                        //切换时保存上下文(寄存器)信息

	char cStart;                        //是否开始，在调用co_resume时设置为1, co_reset时设置为0
	char cEnd;                          //是否结束, co_reset时设置为0
	char cIsMain;                       //是否为主协程, co_init_curr_thread_env中设置为1
	char cEnableSysHook;                //是否hook系统调用 
	char cIsShareStack;                 //是否开启共享栈

	void *pvEnv;                        //环境系统变量指针 TODO

	//char sRunStack[ 1024 * 128 ];     
	stStackMem_t* stack_mem;            //栈指针(私有栈/共享栈)


	//save satck buffer while confilct on same stack_buffer;
	char* stack_sp;                     //TODO
	unsigned int save_size;             //save_buffer大小
	char* save_buffer;                  //用与co_swap切换出共享栈时，保存当前栈信息

	stCoSpec_t aSpec[1024];             //TODO

};



//1.env
void 				co_init_curr_thread_env();
stCoRoutineEnv_t *	co_get_curr_thread_env();

//2.coroutine
void    co_free( stCoRoutine_t * co );
void    co_yield_env(  stCoRoutineEnv_t *env );

//3.func



//-----------------------------------------------------------------------------------------------

struct stTimeout_t;
struct stTimeoutItem_t ;

stTimeout_t *AllocTimeout( int iSize );
void 	FreeTimeout( stTimeout_t *apTimeout );
int  	AddTimeout( stTimeout_t *apTimeout,stTimeoutItem_t *apItem ,uint64_t allNow );

struct stCoEpoll_t;
stCoEpoll_t * AllocEpoll();
void 		FreeEpoll( stCoEpoll_t *ctx );

stCoRoutine_t *		GetCurrThreadCo();
void 				SetEpoll( stCoRoutineEnv_t *env,stCoEpoll_t *ev );

typedef void (*pfnCoRoutineFunc_t)();

#endif

#define __CO_ROUTINE_INNER_H__
