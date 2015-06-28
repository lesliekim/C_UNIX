#pragma once
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <Command.h>
#include <fcntl.h>
using std::string;
using std::vector;

bool Lexer(const string &str_, vector<string> &words);
bool Parser(vector<string> &words,vector<Command> &cmds);
bool Execute(vector<Command> &cmds);
void PrintWords(vector<string> &words);
void PrintCmd(Command &cmd);
