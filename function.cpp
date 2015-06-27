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
	return true;
}

bool Parser(vector<string> &words,vector<Command> &cmds)
{
	vector<string> arg_list;
	for (size_t iter = 0; iter < words.size(); )
	{
		Command cmd;
		if (arg_list.empty())
		{
			cmd.input_ = "stdin";
		}
		else
		{
			cmd.input_ = "pipe";
		}
		cmd.output_ = "stdout";
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
				}
				iter++;

			}
			else if (words[iter] == "|")// pipe
			{
				cmd.output_ = "pipe";
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
	return true;
}
/*
bool Execute(vector<Command> &cmds)
{
	if (cmds.empty())
	{
		return true;
	}
	
	CmdExe cmds_exe;
	size_t pipe_num = cmds.size();
	int **fd = new int*[pipe_num];
	for (size_t i = 0; i < pipe_num; i++)
	{
		fd[i] = new int[2];
		pipe(fd[i]);
	}
	
	for (size_t iter = 0; iter < cmds.size(); iter++)
	{
		int pid = fork();
		if (pid > 0)// parent
		{
			
		}
		else if()
		{
		}
		else
		{
		}
	}
}
*/
void PrintWords()
{
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