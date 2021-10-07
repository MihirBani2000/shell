#include "../headers.h"
#include "../utils/utils.h"
#include "commands.h"

void shell_commands(int bg_flag)
{
    int pid = fork();
    if (pid < 0)
    {
        perror("error while forking");
        return;
    }
    else if (!pid)
    {
        // child process
        setpgid(0, 0);

        // set signals to default for fg processes
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);

        command[num_args] = NULL;
        if (execvp(command[0], command) < 0)
        {
            char errbuf[SMALL_SIZE];
            sprintf(errbuf, "error on executing %s", command[0]);
            perror(errbuf);
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    else
    {
        // parent process
        if (bg_flag)
        { // for background processes
            add_bg_proc(pid, command);
            printf("[%d] %d\n", num_bgjobs, pid);
        }
        else
        { // for foreground processes
            // printf("\nparent pid %d\n", pid);

            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);
            int status;
            // if (tcsetpgrp(STDIN_FILENO, pid) < 0)
            // {
            //     char *err_buf = (char *)malloc(SMALL_SIZE);
            //     sprintf(err_buf, "Error in foreground process - %s", command[0]);
            //     perror(err_buf);
            // }
            tcsetpgrp(STDIN_FILENO, pid);

            waitpid(pid, &status, WUNTRACED);
            tcsetpgrp(STDIN_FILENO, getpgrp());
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

            if (WIFSTOPPED(status))
            {
                add_bg_proc(pid, command);
                printf("\rProcess %s with pid %d has been stopped.\n", command[0], pid);
            }
        }
    }

    return;
}