#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/signal.h>
#include <linux/sched.h>

asmlinkage long sys_sh_task_info(int pidt, char *filename)
{

	struct task_struct *task;
	int flag = 0;
	struct file *file;
	loff_t pos = 0;
	int fd;

	char buffer[2048];

	mm_segment_t old_fs = get_fs();
	set_fs(KERNEL_DS);

	fd = sys_open(filename, O_WRONLY|O_CREAT, 0644);

	for_each_process(task)
	{
		if (pidt==(task->pid))
		{
			flag = 1;
			printk(KERN_WARNING "\nPROCESS: %s\n \
					  PID_NUMBER: %ld\n \
					  PROCESS STATE: %ld\n \
					  PRIORITY: %ld\n \
					  RT_PRIORITY: %ld\n \
					  STATIC PRIORITY: %ld\n \
					  NORMAL PRIORITY: %ld\n",
					  task->comm, \
					  (long)task_pid_nr(task), \
					  (long)task->state, \
					  (long)task->prio, \
					  (long)task->rt_priority, \
					  (long)task->static_prio, \
					  (long)task->normal_prio);

			char *otpt;

			sprintf(buffer, "\nProcess: %s\n \
				PID_NUMBER: %ld\n \
				PROCESS STATE: %ld\n \
				PRIORITY: %ld\n \
				RT_PRIORITY: %ld\n \
				STATIC PRIORITY: %ld\n \
				NORMAL PRIORIY: %ld\n",
				task->comm, (long)task_pid_nr(task), \
				(long)task->state, (long)task->prio, \
				(long)task->rt_priority, (long)task->static_prio, \
				(long)task->normal_prio);

			// Check if given file exists
			if (fd>=0)
			{
				sys_write(fd, buffer, strlen(buffer));
				file = fget(fd);

				if (file)
				{
					vfs_write(file, buffer, strlen(buffer), &pos);
					fput(file);
				}
				sys_close(fd);
			}
			else
			{
				return -ENOENT;
			}
		}
	}

	set_fs(old_fs);

	// Input task pid does not exist
	if (flag==0)
	{
		return -ESRCH;
	}

	return 0;
}
