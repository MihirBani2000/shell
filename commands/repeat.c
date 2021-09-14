#include "../headers.h"
#include "../utils/utils.h"
#include "commands.h"

void repeat()
{
    if (num_args < 3)
    {
        fprintf(stderr, "repeat: invalid arguments\n");
        return;
    }

    int num_repeats = atoi(command[1]);
    if (num_args > 2)
    {
        for (int i = 0; i < num_args - 2; i++)
        {
            strcpy(command[i], command[i + 2]);
        }
        num_args -= 2;
        command[num_args] = NULL;
    }

    while (num_repeats--)
    {
        execute();
    }

    return;
}