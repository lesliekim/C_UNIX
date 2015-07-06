#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <function.h>
#include "Readline.h"
#include "CustomCommand.h"


using std::string;
using std::vector;
int main()
{
	Readline handler;
	
	custom_command_init();
    
	while(1)
	{
		vector<string> words;
		vector<Command> cmds;
		
		char *result = handler.rl_gets();
		if (result == NULL)
		{
			printf("\n");
			break;
		}
		else
		{
			string cmd_line = string(result);
			if (Lexer(cmd_line, words))
			{
				if (Parser(words,cmds))
				{
					int flag = custom_command(cmds);
					if (flag == -1)
					{
						Execute(cmds, cmd_line);
					}
					CheckBackgroundList();
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}


	}

	
	return 0;	
}
