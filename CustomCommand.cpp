#include "CustomCommand.h"
#include <unistd.h>

static map<string,int> custom_command_map;
Job fg_job_;
list<Job> bg_job_;

const char* custom_command_list[] = {
	"cd",
	"jobs",
	"fg",
	"bg"
};

int (*fn[])(vector<string>) = {
	custom_cd,
	custom_jobs,
	custom_fg,
	custom_bg
};

int custom_cd(vector<string> argv)
{
	if (argv.size() == 1)
	{
		return 0;
	}
	string path = argv[1];
	if ((path == "") || (path == "~"))
	{
		path = string("/home/") + getenv("USER") + "/";
	}
	int r_val = chdir(path.c_str());
	if (r_val == -1)
	{
		fprintf(stderr,"error in cd\n");
		return -1;
	}
	else
	{
		return 0;
	}
}

int custom_jobs(vector<string> argv)
{
	
	list<Job>::iterator i = bg_job_.begin();
	for (i = bg_job_.begin(); i != bg_job_.end(); i++)
	{
		string status;
		if ((*i).status_ == 0)
		{
			status = "Running";
		}
		else if ((*i).status_ == 1)
		{
			status = "Stopped";
		}
		else if ((*i).status_ == 2)
		{
			status = "Finished";
		}
		else
		{
			status = "Unknown status";
		}
		printf("[%d] %s  : %s\n",(*i).pid_list_[0], (*i).content_.c_str(), status.c_str());
	}
	
	return 0;
}

int custom_fg(vector<string> argv)
{
	if (bg_job_.empty())
	{
		printf("No background jobs");
		return 1;
	}
	fg_job_ = bg_job_.back();
	bg_job_.pop_back();
	if (fg_job_.status_ == 1)// stopped
	{	
		for (size_t i = 0; i < fg_job_.pid_list_.size(); i++)
		{
			kill(fg_job_.pid_list_[i], SIGCONT);
		}
	}
	
	for (size_t i = 0; i < (fg_job_.pid_list_).size(); i++)
	{
		int status;
		int val = waitpid(fg_job_.pid_list_[i], &status,0);
		if (val <= 0)
		{
			fprintf(stderr,"error while wait for pid:%d\n",fg_job_.pid_list_[i]);
		}
	}
	return 0;
}

int custom_bg(vector<string> argv)
{

	if (bg_job_.empty())
	{
		printf("No background jobs");
		return 1;
	}
	Job &job = bg_job_.back();
	if (job.status_ == 1)// stopped
	{	
		for (size_t i = 0; i < job.pid_list_.size(); i++)
		{
			kill(job.pid_list_[i], SIGCONT);
		}
		job.status_ = 0;
	}
	else
	{
		return 1;
	}
	return 0;
}

int custom_command_init()
{
	for (int i  = 0; i < 4; i ++)
	{
		custom_command_map.insert(std::pair<string,int>(string(custom_command_list[i]), i));
	}
	return 0;
}

int custom_command(vector<Command> &cmds)
{
	map<string, int>::iterator iter;
	iter = custom_command_map.find(cmds[0].name_);
	if (iter == custom_command_map.end())
	{
		return -1;
	}
	
	int (*f)(vector<string>) = fn[iter->second];
	f(cmds[0].coeff_);
	return 0;
}












