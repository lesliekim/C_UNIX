#pragma once
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <list>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <Command.h>
#include <fcntl.h>
#include "CustomCommand.h"

using std::string;
using std::vector;
using std::list;

bool Lexer(const string &str_, vector<string> &words);
bool Parser(vector<string> &words,vector<Command> &cmds);
bool Execute(vector<Command> &cmds, string &cmd_line);
void CheckBackgroundList();
void PrintWords(vector<string> &words);
void PrintCmd(Command &cmd);
