#include "../headers.h"
#include "commands.h"

void pwd()
{
    if (CWD)
        free(CWD);
    CWD = (char *)malloc(BIG_SIZE * sizeof(char));

    if (getcwd(CWD, BIG_SIZE) == NULL)
    {
        perror("Error while obtaining current working directory");
        return;
    }
    printf("%s\n", CWD);
    // fflush(stdout);
    return;
}