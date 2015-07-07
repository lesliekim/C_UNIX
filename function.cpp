#include <function.h>


bool Lexer(const string &str_, vector<string> &words)
{
	char* str;
	const size_t len = str_.size();
	str = new char[len + 1];
	strcpy(str,str_.c_str());
	if (str == NULL)
	{
		return false;
	}

	char *pos_pre = str;
	char *pos = NULL;
	int word_len = 0;
	while(*pos_pre != '\0')
	{
		pos = strchr(pos_pre,' ');
		if (pos == NULL)
		{
			word_len = strlen(pos_pre);
			pos = pos_pre + strlen(pos_pre);
		}
		else
		{
			word_len = pos - pos_pre;
		}
		char *word = new char[word_len + 1];
		strncpy(word, pos_pre, word_len);
		word[word_len] = '\0';
		words.push_back(string(word));
		pos_pre = pos;
		while(*pos_pre == ' ')
		{
			pos_pre++;
		}
	}
	//PrintWords(words);
	return true;
}

bool Parser(vector<string> &words,vector<Command> &cmds)
{
	vector<string> arg_list;
	for (size_t iter = 0; iter < words.size(); )
	{
		Command cmd;
		if (cmds.empty())
		{
			cmd.input_ = "stdin";
			cmd.i_type_ = 0;
		}
		else
		{
			cmd.input_ = "pipe";
			cmd.i_type_ = 1;
		}
		cmd.output_ = "stdout";
		cmd.o_type_ = 0;
		cmd.name_ = words[iter];
		arg_list.push_back(words[iter]);
		iter++;

		while(iter < words.size())
		{
			if (words[iter] == "<")// redirect input
			{
				iter++;
				if (words[iter] == "|" || words[iter] == "&")
				{
					printf(" Invalid input arguments %s\n", words[iter].c_str());
					return false;
				}
				else if (iter == words.size())
				{
					printf(" Input arguments needed!\n");
					return false;
				}
				else
				{
					cmd.input_ = words[iter];
					cmd.i_type_ = 2;
				}
				iter++;
			}
			else if (words[iter] == ">")// redirect output
			{
				iter++;
				if (words[iter] == "|" || words[iter] == "&")
				{
					printf(" Invalid output arguments %s\n", words[iter].c_str());
					return false;
				}
				else if (iter == words.size())
				{
					printf(" Output arguments needed!\n");
					return false;
				}
				else
				{
					cmd.output_ = words[iter];
					cmd.o_type_ = 2;
				}
				iter++;

			}
			else if (words[iter] == "|")// pipe
			{
				cmd.output_ = "pipe";
				cmd.o_type_ = 1;
				iter++;
				if (iter == words.size() || words[iter] != "&")
				{
					break;
				}
			}
			else if (words[iter] == "&")
			{
				for (size_t kk = 0; kk < cmds.size(); kk++)
				{
					cmds[kk].is_background = true;
				}
				cmd.is_background = true;
				iter = words.size();
				break;
			}
			else
			{
				arg_list.push_back(words[iter]);
				iter++;
			}
		}

		for (size_t i = 0; i < arg_list.size(); i++)
		{
			(cmd.coeff_).push_back(arg_list[i]);
		}
		cmds.push_back(cmd);
		arg_list.clear();
	}
	//for (size_t j = 0; j < cmds.size(); j++)
	//{
	//	PrintCmd(cmds[j]);
	//}
	return true;
}

bool Execute(vector<Command> &cmds, string &cmd_line)
{
	if (cmds.empty())
	{
		return true;
	}
	
	size_t pipe_num = cmds.size() - 1;
	int **fd = new int*[pipe_num];
	for (size_t i = 0; i < pipe_num; i++)
	{
		fd[i] = new int[2];
		pipe(fd[i]);
	}
	
	Job job;
	job.content_ = cmd_line;
	//vector<int> pid_list;
	for (int iter = (int)(cmds.size() - 1); iter >= 0; iter--)
	{
		int pid = fork();
		if (pid > 0)// parent
		{
			//pid_list.push_back(pid);
			(job.pid_list_).push_back(pid);
		}
		else if(pid == 0)
		{
			
			if (cmds[iter].i_type_ == 1)// pipe	
			{
				dup2(fd[iter - 1][0], STDIN_FILENO);
				fprintf(stderr,"command %s duplicate pipe to stdin\n", cmds[iter].name_.c_str());
			}
			else if (cmds[iter].i_type_ == 2)// file
			{
				int fid = open(cmds[iter].input_.c_str(), O_RDONLY);
				if (fid == -1)
				{
					fprintf(stderr,"Open file %s error!\n", cmds[iter].input_.c_str());
					exit(-1);
				}
				dup2(fid, STDIN_FILENO);
				close(fid);
				fprintf(stderr,"command %s duplicate %s to stdin\n", cmds[iter].name_.c_str(),cmds[iter].input_.c_str());
				
			}
			
			if (cmds[iter].o_type_ == 1)// pipe	
			{
				dup2(fd[iter][1], STDOUT_FILENO);
				fprintf(stderr,"command %s duplicate pipe to stdout\n", cmds[iter].name_.c_str());
			}
			else if (cmds[iter].o_type_ == 2)// file
			{
				int fid = open(cmds[iter].output_.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0664);
				if (fid == -1)
				{
					fprintf(stderr,"Open file %s error!\n", cmds[iter].output_.c_str());
					exit(-1);
				}
				dup2(fid, STDOUT_FILENO);
				close(fid);
				fprintf(stderr,"command %s duplicate %s to stdout\n", cmds[iter].name_.c_str(),cmds[iter].output_.c_str());
				
			}
			
			for (size_t j = 0; j < pipe_num; j++)// close all pipe
			{
				close(fd[j][0]);
				close(fd[j][1]);
			}
			
			char* argv[cmds[iter].coeff_.size() + 1];
			for (size_t j = 0; j < cmds[iter].coeff_.size(); j++)
			{
				argv[j] = (char*)(cmds[iter].coeff_[j].c_str());
			}
			argv[cmds[iter].coeff_.size()] = NULL;
			int flag = execvp(cmds[iter].name_.c_str(),argv);
			if (flag == -1)
			{
				fprintf(stderr,"command %s execution error:%s!\n",cmds[iter].name_.c_str(), strerror(errno));
			}
			exit(-1);		
		
		}
		else
		{
			fprintf(stderr, "Fork failed!\n");
			return false;
		}
		
	}
	// parent
	for (size_t j = 0; j < pipe_num; j++)
	{
		close(fd[j][0]);
		close(fd[j][1]);
	}
	
	// execution
	if (cmds[0].is_background)
	{
		printf("[%lu] %d\n", bg_job_.size()+1, job.pid_list_[0]);
		bg_job_.push_back(job);
	}
	else
	{
		fg_job_ = job;
		for (size_t i = 0; i < (fg_job_.pid_list_).size(); i++)
		{
			int status;
			int val = waitpid(fg_job_.pid_list_[i], &status,0);
			if (val <= 0)
			{
				fprintf(stderr,"error while wait for pid:%d\n",fg_job_.pid_list_[i]);
			}
		}
		fg_job_.pid_list_.clear();
		fg_job_.status_ = 2;
		fg_job_.content_ = "";
		
	}
	delete []fd;
	return true;
}

void CheckBackgroundList()
{
	list<Job>::iterator iter = bg_job_.begin();
	while(iter != bg_job_.end())
	{
		Job job = *iter;
		if(job.status_ == 0)// running
		{
			while(!job.pid_list_.empty())
			{
				int pid = job.pid_list_[0];
				int status;
				int val = waitpid(pid, &status, WNOHANG);
				if (val == 0)// Not finished
				{
					break;
				}
				else if (val > 0)// finished
				{
					fprintf(stderr,"process %d is waited successfully", pid);
				}
				else
				{
					fprintf(stderr,"waitpid error\n");
				}
				job.pid_list_.erase(job.pid_list_.begin());
			}
			if (job.pid_list_.empty())
			{
				job.status_ = 2;
				printf("[%d] finished\t\t%s\n",job.pid_list_[0],job.content_.c_str());
				iter = bg_job_.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		else
		{
			iter++;
		}
	}
	
}
void PrintWords(vector<string> &words)
{
	for (size_t i = 0; i < words.size(); i++)
	{
		printf("%s ",words[i].c_str());
	}
	printf("\n");
}

void PrintCmd(Command &cmd)
{
	printf("name: %s \n", (cmd.name_).c_str());
	printf("input: %s \n", (cmd.input_).c_str());
	printf("output: %s \n", (cmd.output_).c_str());
	printf("is background execution: %d \n", cmd.is_background);
	for (size_t i = 0; i < (cmd.coeff_).size(); i++)
	{
		printf("%s \n", (cmd.coeff_)[i].c_str());
	}
	/*
	if ((cmd->input_ != "stdin") && (cmd->input_ != "pipe"))
	{
		printf("< %s ", cmd->input_.c_str());
	}
	if (cmd->output_ != "stdout")
	{
		if (cmd->output_ == "pipe")
	}
	*/
}
