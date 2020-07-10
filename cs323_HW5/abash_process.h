// process-stub.h                                 Stan Eisenstat (10/28/16)
//
// Backend for Bsh.  See spec for details.
#ifndef _ABASH_PROCESS_H_
#define _ABASH_PROCESS_H_

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include "abash_parse.h"


// Extract status from value returned by waitpid(); ensure that a process
// that is killed has nonzero status; ignores the possibility of stop/continue.
#define STATUS(x) (WIFEXITED(x) ? WEXITSTATUS(x) : 128+WTERMSIG(x))


// Execute command list CMDLIST and return status of last command executed
int process (CMD *cmdList);

#endif /* _ABASH_PROCESS_H_ */
