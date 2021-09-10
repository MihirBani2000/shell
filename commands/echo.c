#include "../headers.h"
#include "commands.h"

void echo()
{
    int idx = 0;
    while (++idx < num_args)
    {
        printf("%s ", command[idx]);
    }
    printf("\n");
    return;
}