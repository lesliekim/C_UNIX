#include <Readline.h>
static char* line_read = NULL;
char* ReadLine()
{
	char cache[2000];
	if(line_read)
	{
		free(line_read);
		line_read = NULL;
	}

	snprintf(cache, sizeof(cache), "%s@rabbit:%s$ ", getenv("USER"), getcwd(NULL,1000));
	line_read = readline(cache);
	if (line_read && *line_read)
	{
		add_history(line_read);
	}
	return(line_read);
}