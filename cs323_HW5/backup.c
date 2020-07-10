#include "abash_process.h"
int p_simple(CMD *cmd);
int p_pipe(CMD *cmd);
int p_and(CMD *cmd);
int p_or(CMD *cmd);
int p_end(CMD *cmd);

typedef struct pid_array {
    pid_t *pids;
    int size;
    int count;
} pid_array;

const pid_array* new_array() {
    pid_array* arr = malloc(sizeof(pid_array));
    arr->pids = malloc(sizeof(int) * 2048);
    arr->size = 0;
    arr->count = 0;
    return arr;
}

pid_array *children = new_array();
pid_array *subcmds = new_array();

int process (CMD *cmd) {
    if (cmd->type == SIMPLE) {
        return p_simple(cmd);
    } else if (cmd -> type == PIPE) {
        return p_pipe(cmd);
    } else if (cmd -> type == SEP_AND) {
        return p_and(cmd);
    } else if (cmd -> type == SEP_OR) {
        return p_or(cmd);
    } else if (cmd -> type == SEP_END) {
        return p_end(cmd);
    }
    return -1;
}

int setvars (CMD *cmd) {
    int stat;
    for (int i = 0; i < cmd->nLocal; i++) {
        stat = setenv(cmd->locVar[i], cmd->locVal[i], 1);
        if (stat != 0) {
            return stat;
        }
    }
    return 0;
}

int from_prep(CMD *cmd, char* n) {
    if (cmd->fromType == REDIR_IN) {
        return open(cmd->fromFile, O_RDONLY, 0);
    } else if (cmd->fromType == REDIR_HERE) {
        char name[11] = "tempXXXXXX";
        int new_stdin_fd = mkstemp(name);
        strcpy(n, name);
        if (write(new_stdin_fd, cmd->fromFile, strlen(cmd->fromFile) * sizeof(char)) == -1) {
            return -1;
        } else {
            lseek(new_stdin_fd, 0, SEEK_SET);
            return new_stdin_fd;
        }
    } else {
        return STDIN_FILENO;
    }
}

int to_prep(CMD *cmd) {
    if (cmd->toType == REDIR_OUT) {
        return open(cmd->toFile, O_CREAT | O_RDWR | O_TRUNC, 0777);
    } else if (cmd->toType == REDIR_APP) {
        return open(cmd->toFile, O_CREAT | O_RDWR | O_TRUNC | O_APPEND, 0777);
    } else {
        return STDOUT_FILENO;
    }
}

void from_fork(CMD *cmd, int new_stdin_fd, bool child, char* name) {
    if (cmd->fromType == REDIR_IN || cmd->fromType == REDIR_HERE) {
        if (child) {
            int duped_in = dup2(new_stdin_fd, STDIN_FILENO);
        }
        if (cmd->fromType == REDIR_HERE) {
            unlink(name);
            remove(name);
        }
        close(new_stdin_fd);
    }
}

void to_fork(CMD *cmd, int new_stdout_fd, bool child) {
    if (cmd->toType == REDIR_OUT || cmd->toType == REDIR_APP) {
        if (child) {
            int duped_out = dup2(new_stdout_fd, STDOUT_FILENO);
        }
        close(new_stdout_fd);
    }
}

int p_simple(CMD *cmd) {
    char name[11];
    int new_stdin_fd = from_prep(cmd, name);
    int new_stdout_fd = to_prep(cmd);
    int rc = fork();

    if (new_stdin_fd < 0 || new_stdout_fd < 0) {
        return -1;
    }

    if (rc < 0) {
        return(1);
    } else if (rc == 0) {
        from_fork(cmd, new_stdin_fd, true, name);
        to_fork(cmd, new_stdout_fd, true);

        int set = setvars(cmd);
        if (set != 0) {
            return set;
        }

        int stat = execvp(cmd->argv[0], cmd->argv);
        return stat;
    } else {
        from_fork(cmd, new_stdin_fd, false, name);
        to_fork(cmd, new_stdout_fd, false);
        int status;
        pid_t wpid = waitpid(rc, &status, 0);
        return status;
    }
}

int p_pipe(CMD *cmd) {
    int fd_pair[2];
    int e = pipe(fd_pair);  //to get the input and output file descriptors
    if (e != 0) {
        return e;
    }

    int rc = fork();
    if (rc < 0) {
        return(1);
    } else if (rc == 0) {
        int duped = dup2(fd_pair[1], STDOUT_FILENO);
        close(fd_pair[0]);
        close(fd_pair[1]);
        exit(process(cmd->left));
    } else {
        int rc2 = fork();
        if (rc2 < 0) {
            return(1);
        } else if (rc2 == 0) {
            int duped2 = dup2(fd_pair[0], STDIN_FILENO);
            close(fd_pair[0]);
            close(fd_pair[1]);
            exit(process(cmd->right));
        } else {
            close(fd_pair[0]);
            close(fd_pair[1]);
            int status, status2;
            pid_t wpid = waitpid(rc, &status, 0);
            pid_t wpid2 = waitpid(rc2, &status2, 0);
            if (status != 0 || status2 != 0) {
                return -1;
            }
        }
    }
    return 0;
}

CMD* single_rotate(CMD* cmd) {
    // do a left rotate
    return cmd;
}

CMD* rotate(CMD *cmd) {
    CMD* head = cmd;
    while (cmd->right->type == SEP_AND || cmd->right->type == SEP_OR) {
        head = single_rotate(cmd);
    }
    return head;
}


int p_and(CMD *cmd) {
    CMD* head = rotate(cmd);
    int res = process(head->left);
    if (res == 0) {
        return process(head->right);
    } else {
        return res;
    }
}

int p_or(CMD *cmd) {
    CMD* head = rotate(cmd);
    int res = process(head->left);
    if (res != 0) {
        return process(head->right);
    } else {
        return res;
    }
}

int p_end(CMD *cmd) {
    int rc = fork();
    if (rc < 0) {
        return(1);
    } else if (rc == 0) {
        exit(process(cmd->left));
    } else {
        int rc2 = fork();
        if (rc2 < 0) {
            return(1);
        } else if (rc2 == 0) {
            exit(process(cmd->right));
        } else {
            int status, status2;
            pid_t wpid = waitpid(rc, &status, 0);
            pid_t wpid2 = waitpid(rc2, &status2, 0);
            if (status != 0 || status2 != 0) {
                return -1;
            }
        }
    }
    return 0;
}