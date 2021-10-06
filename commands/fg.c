#include "../headers.h"
#include "commands.h"
#include "../utils/utils.h"

void fg()
{
    int job_no, job_pid;
    char *job_name = malloc(BIG_SIZE * sizeof(char));
    int status;

    if (num_args != 2)
    {
        fprintf(stderr, "fg: Invalid arguments\n");
        return;
    }

    job_no = atoi(command[1]);
    if (job_no <= 0)
    {
        fprintf(stderr, "fg: invalid Job number - must be positive\n");
        return;
    }
    else if (job_no > num_bgjobs)
    {
        fprintf(stderr, "fg: invalid Job number - exceeding limit\n");
        return;
    }

    job_pid = bgjobs[job_no].pid;
    strcpy(job_name, bgjobs[job_no].name);

    // for foreground processes
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    if (tcsetpgrp(STDIN_FILENO, job_pid) < 0)
    {
        char *err_buf = (char *)malloc(SMALL_SIZE);
        sprintf(err_buf, "fg: Error in foreground process - %s", job_name);
        perror(err_buf);
    }

    kill(job_pid, SIGCONT);
    waitpid(job_pid, &status, WUNTRACED);
    tcsetpgrp(STDIN_FILENO, getpgrp());
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);

    if (WIFSTOPPED(status))
    { // ctrl Z pressed
        // add_bg_proc(job_pid, command);
        printf("\rProcess %s with pid %d has been stopped.\n", job_name, job_pid);
        return;
    }

    // other than ctrl z , that is process stopped automatically or ctrl C
    for (int i = 1; i <= num_bgjobs; i++)
    {
        // matched job is now removed
        if (bgjobs[i].pid == job_pid)
        {
            // printf("child pid %d\n", pid);
            for (int j = i + 1; j <= num_bgjobs; j++)
            {
                bgjobs[j - 1].pid = bgjobs[j].pid;
                strcpy(bgjobs[j - 1].name, bgjobs[j].name);
            }
            num_bgjobs--;
            break;
        }
    }

    return;
}