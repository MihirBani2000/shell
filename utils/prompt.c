#include "../headers.h"
#include "utils.h"

void show_prompt()
{
    struct passwd *host = getpwuid(getuid());
    char *username, *hostname, *path;

    if (PROMPT)
        free(PROMPT);
    PROMPT = (char *)malloc(BIG_SIZE * sizeof(char));

    username = (char *)malloc(SMALL_SIZE * sizeof(char));
    hostname = (char *)malloc(SMALL_SIZE * sizeof(char));
    path = (char *)malloc(BIG_SIZE * sizeof(char));

    if (host == NULL)
    {
        perror("Error retreiving User and Host Details");
        strcpy(username, "Dummy_name");
        strcpy(hostname, "Dummy_host");
    }
    else
    {
        strcpy(username, host->pw_name);
        if (gethostname(hostname, SMALL_SIZE) < 0)
        {
            perror("Error retreiving Host Details");
            strcpy(hostname, "Dummy_host");
        }
    }

    if (getcwd(CWD, BIG_SIZE) == NULL)
    {
        perror("Error while obtaining current working directory");
        custom_exit(EXIT_FAILURE);
    }

    if (!strstr(CWD, HOME))
    {
        strcpy(path, CWD);
    }
    else
    {
        strcpy(path, "~");
        strcat(path, CWD + strlen(HOME));
    }

    sprintf(PROMPT, "<\033[1;32m%s@%s:\033[1;34m%s\033[0m> ",
            username, hostname, path);

    printf("%s", PROMPT);
    // fflush(stdout);

    free(username);
    free(hostname);
    free(path);

    return;
}