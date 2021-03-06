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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include "co_routine.h"
using namespace std;
struct stTask_t
{
	int id;
};
struct stEnv_t
{
	stCoCond_t* cond;               //条件变量
	queue<stTask_t*> task_queue;    //任务队列
};
void* Producer(void* args)
{
	co_enable_hook_sys();
	stEnv_t* env=  (stEnv_t*)args;
	int id = 0;
	while (true)
	{
		stTask_t* task = (stTask_t*)calloc(1, sizeof(stTask_t));
		task->id = id++;
		env->task_queue.push(task);
		printf("%s:%d produce task %d\n", __func__, __LINE__, task->id);
		co_cond_signal(env->cond);
		poll(NULL, 0, 1000);
	}
	return NULL;
}
void* Consumer(void* args)
{
	co_enable_hook_sys();
	stEnv_t* env = (stEnv_t*)args;
	while (true)
	{
		if (env->task_queue.empty())
		{
			co_cond_timedwait(env->cond, -1);               //阻塞在条件变量，永不超时
			continue;
		}
		stTask_t* task = env->task_queue.front();
		env->task_queue.pop();
		printf("%s:%d consume task %d\n", __func__, __LINE__, task->id);
		free(task);
	}
	return NULL;
}
int main()
{
	stEnv_t* env = new stEnv_t;                             //stEnv_t 包含本示例使用的条件变量和任务队列
	env->cond = co_cond_alloc();                            //分配条件变量实例

	stCoRoutine_t* consumer_routine;                        //声明消费者协程结构
	co_create(&consumer_routine, NULL, Consumer, env);      //创建消费者协程
	co_resume(consumer_routine);                            //启动消费者协程

	stCoRoutine_t* producer_routine;                        //声明生产者协程结构
	co_create(&producer_routine, NULL, Producer, env);      //创建生产者协程
	co_resume(producer_routine);                            //启动生产者协程
	
	co_eventloop(co_get_epoll_ct(), NULL, NULL);            //事件循环, 触发超时与epoll_wait
	return 0;
}
