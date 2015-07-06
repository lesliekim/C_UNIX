#pragma once
#include <string>
#include <vector>

class Readline
{
public:
    Readline();
    ~Readline();
    char* rl_gets();
    char *line_read_;
};



