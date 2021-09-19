#include "stdio.h"
#include "string.h"
#include "stdlib.h"

int main()
{
    char *prp;
    prp = (char *)malloc(100);
    strcpy(prp, "mihir");
    printf("prp %s\n", prp);
    // free(prp);

    // if (prp)
    // {
    //     printf("prp");
    //     free(prp);
    // }

    // prp = (char *)malloc(100);
    strcpy(prp, "bani");
    // prp = "mihir";
    printf("prp %s\n", prp);
    free(prp);
    printf("\b");

    char *com[10];
    if (com)
    {
        // free(com);
        printf("hi\n");
    }

    printf("\b");
    for (int i = 0; i < 10; i++)
    {
        com[i] = (char *)malloc(100);
        strcpy(com[i], "string no - ");
        printf("%s\n", com[i]);
    }

    printf("freeing\n");

    for (int i = 0; i < 10; i++)
    {
        free(com[i]);
    }
    // free(com);

    for (int i = 0; i < 10; i++)
    {
        com[i] = (char *)malloc(100);
        strcpy(com[i], "fadfas string no - ");
        // strcat(com[i], (char)i);
        printf("\b%s\t", com[i]);
    }
    return 0;
}