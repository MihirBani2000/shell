#include "../headers.h"
#include "commands.h"

void repeat()
{
    int idx = 0;
    while (command[++idx] != NULL)
    {
        printf("%s ", command[idx]);
    }
    printf("\n");
}