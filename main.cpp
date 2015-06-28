#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <function.h>
#include "Readline.h"



using std::string;
using std::vector;
int main()
{
	Readline handler;
    
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
					Execute(cmds);
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
