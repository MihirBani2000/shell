#include "../headers.h"
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
    }
    else
    {
        // parent process
        if (bg_flag)
        {
            // for background processes
        }
        else
        {
            // for foreground processes
        }
    }

    return;
}