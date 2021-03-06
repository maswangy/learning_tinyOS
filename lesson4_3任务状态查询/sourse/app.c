#include "tinyOS.h"



tTask tTask1;
tTask tTask2;
tTask	tTask3;
tTask tTask4;

tTastStack task1Env[1024];
tTastStack task2Env[1024];
tTastStack task3Env[1024];
tTastStack task4Env[1024];

int task1flag;

void task1_clean_func(void * param)
{
	task1flag = 0;
}

void task1Entry (void *param)
{

	tSetSysTickPeriod(10);
	//初始化任务被删除的时候回调清理函数
	t_task_set_clean_call_func(&tTask1, task1_clean_func, (void*)0);
	
	for(;;)
	{
		task1flag = 0;
		tTaskDelay(1);
		task1flag = 1;
		tTaskDelay(1);
	}

}

// 

int task2flag;
void task2Entry (void *param)
{
	int task1_delete = 0;
	for(;;)
	{
		
		task2flag = 0;
		tTaskDelay(1);
		task2flag = 1;
		tTaskDelay(1);
		if(!task1_delete)
		{
			t_task_force_delete(&tTask1);
			task1_delete = 1;
		}
	}
}

int task3flag;
void task3Entry (void *param)
{
	for(;;)
	{
		task3flag = 0;
		tTaskDelay(1);
		task3flag = 1;
		tTaskDelay(1);
		if(t_task_is_requested_delete())
		{
			//此处调用清理函数
			task3flag = 0;
			
			//然后删除自己
			t_task_delete_self();
			
		}
	}
}

int task4flag;
void task4Entry (void *param)
{
	uint8_t task3delete = 0;
	for(;;)
	{
		task4flag = 0;
		tTaskDelay(1);
		task4flag = 1;
		tTaskDelay(1);
		if(!task3delete)
		{
			t_task_request_delete(&tTask3);
			task3delete = 1;
		}
	}
}

void app_init()
{
	// 初始化任务1和任务2结构，传递运行的起始地址，想要给任意参数，以及运行堆栈空间
	tTaskInit(&tTask1, task1Entry, (void* )0x11111111, 0, &task1Env[1024]);
	tTaskInit(&tTask2, task2Entry, (void *)0x22222222, 1, &task2Env[1024]);
	
	tTaskInit(&tTask3, task3Entry, (void *)0x33333333, 0, &task3Env[1024]);
	tTaskInit(&tTask4, task4Entry, (void *)0x44444444, 1, &task4Env[1024]);

}




