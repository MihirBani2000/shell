#include "../headers.h"
#include "commands.h"

void cd()
{
    if (getcwd(CWD, BIG_SIZE) == NULL)
    {
        perror("getcwd: Error while obtaining current working directory");
        return;
    }

    char *path = (char *)malloc(BIG_SIZE * sizeof(char));

    if (num_args == 1)
    {
        // printf("hi\n");
        // no arg given to cd command, switch to home
        if ((chdir(HOME) != 0))
        {
            perror("cd: Error while changing to Home directory");
            return;
        }
    }
    else if (num_args > 2)
    {
        printf("cd: Excess arguments given\n");
        return;
    }
    else if (command[1][0] == '~')
    {
        strcpy(path, HOME);
        strcat(path, command[1] + 1);
        if (chdir(path) != 0)
        {
            perror("cd: Error while changing directory");
            return;
        }
    }
    else if (command[1][0] == '-')
    {
        if (strstr(LAST_WD, HOME) != NULL)
        {
            strcpy(path, "~");
            strcat(path, LAST_WD + strlen(HOME));
        }
        else
        {
            strcpy(path, LAST_WD);
        }

        if (chdir(LAST_WD) != 0)
        {
            perror("cd: Error while changing directory");
            return;
        }

        printf("%s\n", path);
    }
    else
    {
        if (chdir(command[1]) != 0)
        {
            perror("cd: Error while changing directory");
            return;
        }
    }

    strcpy(LAST_WD, CWD);
    free(path);
    return;
}