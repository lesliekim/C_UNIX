#include "ShortCut.h"

sigjmp_buf jmpbuf_ctrlz;
sigjmp_buf jmpbuf_ctrlc;

void bind()
{
	struct sigaction act;
	sigfillset(&(act.sa_mask));
	act.sa_flags = SA_SIGINFO;
	
	act.sa_sigaction = ctrl_z;
	sigaction(SIGTSTP, &act, NULL);
	
	act.sa_sigaction = ctrl_c;
	sigaction(SIGINT, &act, NULL);
}

void ctrl_z(int signo, siginfo_t *info, void *ctx)
{
	
	if (!fg_job_.pid_list_.empty())
	{
		Job job;
		job = fg_job_;
		fg_job_.pid_list_.clear();
		job.status_ = 1;
		bg_job_.push_back(job);
	}
	
	for (auto i = bg_job_.begin(); i != bg_job_.end(); i++)
	{
		i->status_ = 1;
	}
	siglongjmp(jmpbuf_ctrlz,1);
}

void ctrl_c(int signo, siginfo_t *info, void *ctx)
{
	for (auto i = fg_job_.pid_list_.begin(); i != fg_job_.pid_list_.end(); i++)
	{
		int status;
		waitpid((*i), &status, 0);
	}
	
	for (auto j = bg_job_.begin(); j != bg_job_.end(); j++)
	{
		Job job;
		job = (*j);
		
		for (auto k = job.pid_list_.begin(); k != job.pid_list_.end(); k++)
		{
			int pid = (*k);
			int status;
			kill(pid, SIGKILL);
			waitpid(pid,&status,0);
		}
	}
	bg_job_.clear();
	siglongjmp(jmpbuf_ctrlc,1);
}








