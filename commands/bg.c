#include "../headers.h"
#include "commands.h"

void bg()
{
    int job_no, job_pid;
    if (num_args != 2)
    {
        fprintf(stderr, "bg: Invalid arguments\n");
        return;
    }

    job_no = atoi(command[1]);
    if (job_no <= 0)
    {
        fprintf(stderr, "bg: invalid Job number - must be positive\n");
        return;
    }
    else if (job_no > num_bgjobs)
    {
        fprintf(stderr, "bg: invalid Job number - exceeding limit\n");
        return;
    }

    job_pid = bgjobs[job_no].pid;

    if (kill(job_pid, SIGCONT) != 0)
    {
        perror("bg: error in running the process");
    }

    return;
}