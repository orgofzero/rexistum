#include <kernel/task.h>
#include <kernel/memory.h>
#include <kernel/string.h>
#include <arch/x86/asmfunc.h>

struct task_info task_list[1024];
int now_task_pid = 0; //当前运行的任务pid
/* 初始化多任务 */
void init_task()
{
	now_task_pid = 0;
	int i = 0;
	for(; i < 1024; i++)
	{
		task_list[i].flags = 0;
	}
}
/* 获取下一个任务pid */
int GetNextPid()
{
	int i = now_task_pid + 1;
	for(; i < 1024; i++)
	{
		if(task_list[i].flags == 1)
		{
			return i;
		}
	}
	for(i = 0; i < now_task_pid; i++)
	{
		if(task_list[i].flags == 1)
		{
			return i;
		}
	}
	return -1; //没有正在运行的任务
}
/* 切换任务 */
void SwitchTask()
{
	int pid = GetNextPid();
	if(pid != -1)
	{
		int old_pid = now_task_pid;
		now_task_pid = pid;
		asm_SwitchTask(&task_list[old_pid].status, &task_list[pid].status);
	}
}
/* 创建任务 */
int CreateTask(unsigned int addr)
{
	int i = 0;
	for(; i < 1024; i++)
	{
		if(task_list[i].flags == 0)
		{
			unsigned int esp_addr = AllocMemfrag(1024) + 1024; //分配该任务的栈地址
			task_list[i].flags = 1;
			task_list[i].name[0] = '\0';
			/* 初始化寄存器 */
			task_list[i].status.eax = 0;
			task_list[i].status.ebx = 0;
			task_list[i].status.ecx = 0;
			task_list[i].status.edx = 0;
			task_list[i].status.esi = 0;
			task_list[i].status.edi = 0;
			task_list[i].status.ebp = 0;
			task_list[i].status.esp = esp_addr;
			int *p = (int*)esp_addr;
			*p = addr; //[esp]为任务跳转地址
			return i; //返回pid
		}
	}
	return -1;
}
/* 为当前执行的代码创建任务 */
int CreateCurrentTask()
{
	int i = 0;
	for(; i < 1024; i++)
	{
		if(task_list[i].flags == 0)
		{
			/* 因为当前任务被挂起时状态会被保存,所以不用初始化寄存器 */
			task_list[i].flags = 1;
			task_list[i].name[0] = '\0';
			return i; //返回pid
		}
	}
	return -1;
}
/* 设置任务名字 */
void SetTaskName(int pid, char *str)
{
	str_cpy(task_list[pid].name, str);
}
/* 获取任务名字 */
void GetTaskName(char *ret, int pid)
{
	str_cpy(ret, task_list[pid].name);
}
/* 杀死任务 */
void KillTask(int pid)
{
	task_list[pid].flags = 0;
}
/* 获取任务pid列表 */
int ListTask(int *ret)
{
	int i, j;
	j = 0;
	for(i = 0; i < 1024; i++)
	{
		if(task_list[i].flags == 1)
		{
			ret[j] = i;
			j += 1;
		}
	}
	return j + 1;
}