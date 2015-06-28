#include "Readline.h"
#include <unistd.h>
#include <string>
#include <readline/readline.h>
#include <readline/history.h>

using std::string;


Readline::Readline(): line_read_(NULL)
{
}

Readline::~Readline()
{
    if (line_read_)
    {
        free(line_read_);
    }
}

char* Readline::rl_gets()
{
    if (line_read_)
    {
        free(line_read_);
        line_read_ = NULL;
    }

    string shell_prompt;
	shell_prompt = getenv("USER") + string("@") + "rabbit:"
            + getcwd(NULL, 1000) + "$ ";
    line_read_ = readline(shell_prompt.c_str());

    if (line_read_ && *line_read_)
    {
        add_history(line_read_);
    }

    return line_read_;
}

