#include "abash_process.h"

int p_simple(CMD *cmd);
int p_pipe(CMD *cmd);
int p_sep(CMD *cmd);
int p_bg(CMD *cmd);
int p_subcmd(CMD *cmd);
int p_command(CMD *cmd);
int builtin_cd(CMD *cmd);
int builtin_dirs(CMD *cmd);
int builtin_wait(CMD *cmd);

void err_check() {
    if (errno != 0) {
        perror("Error");
    }
}

int process(CMD *cmd) {
    pid_t child = 1;
    while (true) {
        int status;
        child = waitpid(-1, &status, WNOHANG);
        if (child > 0) {
            printf("Completed: %d\n", child);
        } else {
            break;
        }
    }
    errno = 0;

    return p_command(cmd);
}

int p_command(CMD *cmd) {
    if (cmd->argc > 0 && strcmp(cmd->argv[0], "cd") == 0) {
        return builtin_cd(cmd);
    } else if (cmd->argc > 0 && strcmp(cmd->argv[0], "wait") == 0) {
        return builtin_wait(cmd);
    } else if (cmd->type == SIMPLE) {
        return p_simple(cmd);
    } else if (cmd->type == PIPE) {
        return p_pipe(cmd);
    } else if (cmd->type == SEP_AND || cmd->type == SEP_OR || cmd->type == SEP_END) {
        return p_sep(cmd);
    } else if (cmd->type == SEP_BG) {
        return p_bg(cmd);
    } else if (cmd->type == SUBCMD) {
        return p_subcmd(cmd);
    }
    return 0;
}

void setvars (CMD *cmd) {
    for (int i = 0; i < cmd->nLocal; i++) {
        setenv(cmd->locVar[i], cmd->locVal[i], 1);
    }
}

int from_prep(CMD *cmd, char* n) {
    if (cmd->fromType == REDIR_IN) {
        int desc = open(cmd->fromFile, O_RDONLY, 0);
        err_check();
        return desc;
    } else if (cmd->fromType == REDIR_HERE) {
        char name[11] = "tempXXXXXX";
        int new_stdin_fd = mkstemp(name);
        strcpy(n, name);
        write(new_stdin_fd, cmd->fromFile, strlen(cmd->fromFile) * sizeof(char));
        lseek(new_stdin_fd, 0, SEEK_SET);
        return new_stdin_fd;
    } else {
        return STDIN_FILENO;
    }
}

int to_prep(CMD *cmd) {
    if (cmd->toType == REDIR_OUT) {
        int desc = open(cmd->toFile, O_CREAT | O_RDWR | O_TRUNC, 0777);
        err_check();
        return desc;
    } else if (cmd->toType == REDIR_APP) {
        int desc = open(cmd->toFile, O_CREAT | O_RDWR | O_TRUNC | O_APPEND, 0777);
        err_check();
        return desc;
    } else {
        return STDOUT_FILENO;
    }
}

void from_fork(CMD *cmd, int new_stdin_fd, bool child, char* name) {
    if (cmd->fromType == REDIR_IN || cmd->fromType == REDIR_HERE) {
        if (child) {
            dup2(new_stdin_fd, STDIN_FILENO);
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
            dup2(new_stdout_fd, STDOUT_FILENO);
        }
        close(new_stdout_fd);
    }
}

int p_simple(CMD *cmd) {
    char name[11];
    int new_stdin_fd = from_prep(cmd, name);
    if (errno != 0) {
        return errno;
    }
    int new_stdout_fd = to_prep(cmd);
    if (errno != 0) {
        return errno;
    }
    int rc = fork();
    err_check();
    if (errno != 0) {
        return errno;
    }

    if (rc < 0) {
        return(errno);
    } else if (rc == 0) {
        from_fork(cmd, new_stdin_fd, true, name);
        to_fork(cmd, new_stdout_fd, true);
        setvars(cmd);
        if (strcmp(cmd->argv[0], "dirs") == 0) {
            builtin_dirs(cmd);
        } else {
            execvp(cmd->argv[0], cmd->argv);
            err_check();
            if (errno != 0) {
                return errno;
            }
        }
        exit(errno);
    } else {
        from_fork(cmd, new_stdin_fd, false, name);
        to_fork(cmd, new_stdout_fd, false);
        int status;
        waitpid(rc, &status, 0);

        char str[12];
        sprintf(str, "%d", STATUS(status));

        setenv("?", str, 1);
        return STATUS(status);
    }
}

int p_pipe(CMD *cmd) {
    int fd_pair[2];
    pipe(fd_pair);
    err_check();
    if (errno != 0) {
        return errno;
    }

    int rc = fork();
    err_check();
    if (errno != 0) {
        return errno;
    }
    if (rc < 0) {
        return(errno);
    } else if (rc == 0) {
        dup2(fd_pair[1], STDOUT_FILENO);
        close(fd_pair[0]);
        close(fd_pair[1]);
        exit(p_command(cmd->left));
    } else {
        int rc2 = fork();
        if (rc2 < 0) {
            return(errno);
        } else if (rc2 == 0) {
            dup2(fd_pair[0], STDIN_FILENO);
            close(fd_pair[0]);
            close(fd_pair[1]);
            exit(p_command(cmd->right));
        } else {
            close(fd_pair[0]);
            close(fd_pair[1]);
            int status, status2;
            waitpid(rc, &status, 0);
            waitpid(rc2, &status2, 0);
            if (STATUS(status2) != 0) {
                char str[12];
                sprintf(str, "%d", STATUS(status2));

                setenv("?", str, 1);
                return STATUS(status2);
            } else {
                char str[12];
                sprintf(str, "%d", STATUS(status));

                setenv("?", str, 1);
                return STATUS(status);
            }

        }
    }
    return 0;
}

CMD* rotate_once(CMD* cmd) {
    CMD* b = cmd->right;
    cmd->right = b->left;
    b->right = cmd; 
    return b;
}

void rotate(CMD *cmd) {
    while (cmd->right->type == SEP_AND || cmd->right->type == SEP_OR) {
        cmd = rotate_once(cmd);
    }
}

int p_sep(CMD *cmd) {
    if (cmd->type == SEP_OR || cmd->type == SEP_AND) {
        rotate(cmd);
    }

    int status = p_command(cmd->left);

    if (cmd->type == SEP_OR && status == 0) {
        return 0;
    }

    if (cmd->type == SEP_AND && status != 0) {
        return 0;
    }

    return p_command(cmd->right);
}

int p_bg(CMD *cmd) {
    int rc = fork();
    err_check();
    if (errno != 0) {
        return errno;
    }
    if (rc < 0) {
        return(errno);
    } else if (rc == 0) {
        printf("Backgrounded: %d\n", rc);
        exit(p_command(cmd->left));
    } else {
        return p_command(cmd->right);
    }
}

int p_subcmd(CMD *cmd) {
    char name[11];
    int new_stdin_fd = from_prep(cmd, name);
    int new_stdout_fd = to_prep(cmd);
    int rc = fork();
    err_check();
    if (errno != 0) {
        return errno;
    }
    if (new_stdin_fd < 0 || new_stdout_fd < 0) {
        return -1;
    }

    if (rc < 0) {
        return(errno);
    } else if (rc == 0) {
        from_fork(cmd, new_stdin_fd, true, name);
        to_fork(cmd, new_stdout_fd, true);
        setvars(cmd);
        p_command(cmd->left);
        exit(errno);
    } else {
        from_fork(cmd, new_stdin_fd, false, name);
        to_fork(cmd, new_stdout_fd, false);
        int status;
        waitpid(rc, &status, 0);
        char str[12];
        sprintf(str, "%d", STATUS(status));

        setenv("?", str, 1);
        return STATUS(status);
    }
}

// builtins

int builtin_cd(CMD* cmd) {
    if (cmd->argc == 2) {
        chdir(cmd->argv[1]);
    } else if (cmd->argc > 2) {
        errno = 7;
    } else {
        chdir(getenv("HOME"));
    }
    err_check();
    
    char str[12];
    sprintf(str, "%d", errno);
    setenv("?", str, 1);
    return errno;
}

int builtin_wait(CMD* cmd) {
    pid_t child = 1;
    while (true) {
        int status;
        child = waitpid(-1, &status, WNOHANG);
        if (child != 0) {
            break;
        }
    }

    return errno;
}

int builtin_dirs(CMD* cmd) {
    char name[256];
    printf(getcwd(name, 255));
    printf("\n");
    err_check();
    return errno;
}
