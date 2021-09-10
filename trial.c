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

    if (prp)
    {
        printf("prp");
        free(prp);
    }

    prp = (char *)malloc(100);
    strcpy(prp, "mihir");
    // prp = "mihir";
    printf("prp %s\n", prp);
    free(prp);

    char *com[10];
    if (com)
    {
        // free(com);
        printf("hi\n");
    }

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
        printf("%s\n", com[i]);
    }
    return 0;
}