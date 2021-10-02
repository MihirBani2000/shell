#include "../headers.h"
#include "commands.h"

void sig()
{
    int job_no, job_pid, signal;
    if (num_args != 3)
    {
        fprintf(stderr, "sig: Invalid arguments\n");
        return;
    }

    job_no = atoi(command[1]);
    if (job_no <= 0)
    {
        fprintf(stderr, "sig: invalid Job number - must be positive\n");
        return;
    }
    else if (job_no > num_bgjobs)
    {
        fprintf(stderr, "sig: invalid Job number - exceeding limit\n");
        return;
    }

    signal = atoi(command[2]);
    if ((signal < 1) || (signal > 64) || (signal == 16) || (signal == 32) || (signal == 33))
    {
        fprintf(stderr, "sig: invalid signal number - should be in [1,15] [17,31] [34,64]\n");
        return;
    }

    job_pid = bgjobs[job_no].pid;
    if (kill(job_pid, signal) != 0)
    {
        perror("sig: error in sending the signal");
    }
    return;
}