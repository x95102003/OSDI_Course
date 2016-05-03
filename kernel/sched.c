#include <kernel/task.h>
#include <inc/x86.h>

#define ctx_switch(ts) \
  do { env_pop_tf(&((ts)->tf)); } while(0)

/* TODO: Lab5
* Implement a simple round-robin scheduler (Start with the next one)
*
* 1. You have to remember the task you picked last time.
*
* 2. If the next task is in TASK_RUNNABLE state, choose
*    it.
*
* 3. After your choice set cur_task to the picked task
*    and set its state, remind_ticks, and change page
*    directory to its pgdir.
*
* 4. CONTEXT SWITCH, leverage the macro ctx_switch(ts)
*    Please make sure you understand the mechanism.
*/
void sched_yield(void)
{
	extern Task tasks[];
	extern Task *cur_task;
	Task *last_task = cur_task;
	size_t i;
	for(i=0;i<NR_TASKS;i++)
	{
		if(tasks[i].state == TASK_RUNNABLE)
		{
			if(cur_task->state != TASK_SLEEP)
				cur_task->state = TASK_RUNNABLE;
			tasks[i].remind_ticks = TIME_QUANT;
			tasks[i].state = TASK_RUNNING;
			cur_task = &tasks[i];
			break;
		}	
	}
	if(last_task->remind_ticks <=0 && last_task->state == TASK_RUNNING)
		last_task->state = TASK_RUNNABLE;
	lcr3(PADDR(cur_task->pgdir));
	ctx_switch(cur_task);
}
