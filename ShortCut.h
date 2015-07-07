#pragma once
#include <signal.h>
#include <setjmp.h>
#include <sys/wait.h>
#include "Command.h"
#include "CustomCommand.h"

extern sigjmp_buf jmpbuf_ctrlz;
extern sigjmp_buf jmpbuf_ctrlc;


void ctrl_z(int signo, siginfo_t *info, void *ctx);
void ctrl_c(int signo, siginfo_t *info, void *ctx);
void bind();
