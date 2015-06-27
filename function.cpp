#include <cstdio>

bool Lexer(const string &str, vector<string> &words)
{
	if (str == NULL)
	{
		return false;
	}

	char *pos_pre = str.c_str();
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
	for (size_t iter = 0; size_t iter < words.size(); )
	{
		cmd = new Command;
		if (arg_list.empty())
		{
			cmd->input = "stdin";
		}
		else
		{
			cmd->input = "pipe"
		}
		cmd->output = "stdout";
		cmd->name = words(iter);
		arg_list.push_back(words(iter));
		iter++;

		while(iter < words.size())
		{
			if (words(iter) == "<")// redirect input
			{
				iter++;
				if (words(iter) == "|" || words(iter) == "&")
				{
					printf(" Invalid input arguments %s\n", words(iter).c_str());
					return false;
				}
				else if (iter == words.size())
				{
					printf(" Input arguments needed!\n");
					return false;
				}
				else
				{
					cmd->input = words(iter);
				}
				iter++;
			}
			else if (words(iter) == ">")// redirect output
			{
				iter++;
				if (words(iter) == "|" || words(iter) == "&")
				{
					printf(" Invalid output arguments %s\n", words(iter).c_str());
					return false;
				}
				else if (iter == words.size())
				{
					printf(" Output arguments needed!\n");
					return false;
				}
				else
				{
					cmd->output = words(iter);
				}
				iter++;

			}
			else if (words(iter) == "|")// pipe
			{
				cmd->output = "pipe";
				iter++;
				if (iter == words.size() || words(iter) != "&")
				{
					break;
				}
			}
			else if (words(iter) == "&")
			{
				for (size_t kk = 0; kk < cmds.size(); kk++)
				{
					cmds(kk).is_background = true;
				}
				cmd->is_background = true;
				iter = words.size();
				break;
			}
			else
			{
				arg_list.push_back(words(iter));
				iter++;
			}
		}

	}

}