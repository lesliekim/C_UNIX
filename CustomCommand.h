#pragma once
#include <string>
#include <map>
#include <sys/wait.h>
#include <Command.h>

using std::map;
using std::vector;

extern Job fg_job_;
extern list<Job> bg_job_;

int custom_cd(vector<string> argv);
int custom_jobs(vector<string> argv);
int custom_fg(vector<string> argv);
int custom_bg(vector<string> argv);
int custom_command_init();
int custom_command(vector<Command> &cmds);
