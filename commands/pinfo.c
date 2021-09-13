#include "../headers.h"
#include "commands.h"

void pinfo()
{
    pid_t pid;
    if (num_args == 1)
    {
        pid = getpid();
    }
    else if (num_args == 2)
    {
        pid = atoi(command[1]);
    }
    else
    {
        printf("pinfo: Invalid arguments\n");
        return;
    }

    char *stat_file_name = (char *)malloc(SMALL_SIZE * sizeof(char));
    char *exe_file_name = (char *)malloc(SMALL_SIZE * sizeof(char));
    sprintf(stat_file_name, "/proc/%d/status", pid);
    sprintf(exe_file_name, "/proc/%d/exe", pid);

    FILE *stat_file;
    char *stat_buffer = (char *)malloc(BIG_SIZE * sizeof(char));
    char *exe_buffer = (char *)malloc(BIG_SIZE * sizeof(char));

    stat_file = fopen(stat_file_name, "r");
    if (stat_file == NULL)
    {
        char *err_buf = (char *)malloc(SMALL_SIZE * sizeof(char));
        sprintf(err_buf, "Process with pid: %d doesn't exist", pid);
        perror(err_buf);
        free(err_buf);
        return;
    }

    char *p_status = (char *)malloc(SMALL_SIZE * sizeof(char));
    char *p_exepath = (char *)malloc(BIG_SIZE * sizeof(char));
    long p_memory;
    while (fgets(stat_buffer, BIG_SIZE, stat_file))
    {
        char *token = strtok(stat_buffer, " \t\r");
        if (strcmp(token, "State:") == 0)
        {
            token = strtok(NULL, " \r\t");
            strcpy(p_status, token);
        }
        if (strcmp(token, "VmSize:") == 0)
        {
            token = strtok(NULL, " \r\t");
            p_memory = atol(token);
        }
    }

    printf("pid -- %d\n", pid);
    printf("Process status -- %s\n", p_status);
    printf("memory -- %ld KB\n", p_memory);

    int written_size = readlink(exe_file_name, exe_buffer, BIG_SIZE);
    if (written_size < 0)
    {
        char *err_buf = (char *)malloc(SMALL_SIZE * sizeof(char));
        sprintf(err_buf, "pinfo: Unable to read from %s", exe_file_name);
        perror(err_buf);
        free(err_buf);
        return;
    }
    else
    {
        exe_buffer[written_size] = '\0';
        char *home_sub = strstr(exe_buffer, HOME);
        if (home_sub == NULL)
        {
            strcpy(p_exepath, exe_buffer);
        }
        else
        {
            strcpy(p_exepath, "~");
            strcat(p_exepath, home_sub + strlen(HOME));
            strcat(p_exepath, "\0");
        }
        printf("Executable Path -- %s\n", p_exepath);
    }

    // printf("\n\nhi bhai\n\n");
    fclose(stat_file);
    free(p_status);
    free(p_exepath);
    free(stat_file_name);
    free(exe_file_name);
    free(stat_buffer);
    free(exe_buffer);
    return;
}
