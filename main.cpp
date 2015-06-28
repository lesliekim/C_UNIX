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
		
		string cmd_line = string(handler.rl_gets());
		if (cmd_line == "")
		{
			printf("\n");
			break;
		}
		else
		{
			if (Lexer(cmd_line, words))
			{
				if (Parser(words,cmds))
				{

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
