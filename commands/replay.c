#include "../headers.h"
#include "../utils/utils.h"
#include "commands.h"

int check_flags(int *interval, int *period)
{
    // returns < 0 for errors

    int c_flag = 0;
    int c_arg_flag = 0;
    int i_flag = 0;
    int p_flag = 0;

    int i = 0;
    while (i < num_args - 1)
    {
        if (i == 0)
            strcpy(command[i], " ");

        if (!strcmp(command[i], "-command"))
        {
            c_flag++;
            strcpy(command[i], " ");
            // a loop for finding the command
            int idx = i + 1;
            while ((command[idx] != NULL) && (idx < num_args))
            {
                c_arg_flag++;
                if (idx < num_args - 1)
                {
                    if (command[idx + 1] == NULL)
                        break;
                    if (!strcmp(command[idx + 1], "-interval") || !strcmp(command[idx + 1], "-period"))
                        break;
                }
                idx++;
            }
            i = idx;
        }
        else if (!strcmp(command[i], "-interval"))
        {
            i_flag++;
            strcpy(command[i], " ");
            *interval = atoi(command[++i]);
            strcpy(command[i], " ");
        }
        else if (!strcmp(command[i], "-period"))
        {
            p_flag++;
            strcpy(command[i], " ");
            *period = atoi(command[++i]);
            strcpy(command[i], " ");
        }
        i++;
    }

    if ((c_flag != 1) && (i_flag != 1) && (p_flag != 1))
    {
        fprintf(stderr, "replay: Invalid arguments\n");
        return -1;
    }
    if ((*interval <= 0) || (*period <= 0) || (c_arg_flag <= 0))
    {
        // printf("int %d per %d c %d\n", *interval, *period, c_arg_flag);
        fprintf(stderr, "replay: Invalid values for flags\n");
        return -1;
    }

    return 1;
}

void update_execute_command(int interval, int period)
{
    int count = 0;
    char *temp_command[BIG_SIZE];

    for (int i = 0; i < num_args; i++)
    {
        if (strcmp(command[i], " "))
        {
            temp_command[count] = malloc(BIG_SIZE * sizeof(char));
            strcpy(temp_command[count++], command[i]);
            strcpy(command[i], "\0");
        }
    }

    temp_command[count] = NULL;

    for (int i = 0; i < BIG_SIZE; i++)
    {
        if (command[i])
            free(command[i]);
        command[i] = (char *)malloc(BIG_SIZE * sizeof(char));
    }

    num_args = count;
    command[num_args] = NULL;

    for (int i = 0; i < num_args; i++)
    {
        strcpy(command[i], temp_command[i]);
    }

    if (num_args > 0)
    {
        int num = 0;
        int remainder = period % interval;
        num = period / interval;
        for (int idx = 0; idx < num; idx++)
        {
            sleep(interval);
            execute();
            printf("\n");
        }
        sleep(remainder);
    }

    // pid_t pid = fork();
    // if (pid < 0)
    // {
    //     perror("replay: error while forking");
    //     return;
    // }
    // else if (!pid)
    // {
    //     // child process
    //     setpgid(0, 0);

    //     // set signals to default for fg processes
    //     signal(SIGINT, SIG_DFL);
    //     signal(SIGTSTP, SIG_DFL);

    //     if (num_args > 0)
    //     {
    //         int num = 0;
    //         int remainder = period % interval;
    //         num = period / interval;
    //         for (int idx = 0; idx < num; idx++)
    //         {
    //             sleep(interval);
    //             execute();
    //             printf("\n");
    //         }
    //         sleep(remainder);
    //     }
    //     printf("\nreplay finised\n");
    //     exit(1);
    // }
    // else
    // {
    //     // add_bg_proc(pid, command);
    //     return;
    // }

    return;
}

void replay()
{
    // replay -command <command> -interval <interval> -period <period>
    if (num_args < 7)
    {
        fprintf(stderr, "replay: invalid arguments\n");
        return;
    }

    int i = -1, p = -1;

    int flag = check_flags(&i, &p);
    if (flag > 0)
    {
        update_execute_command(i, p);
    }

    return;
}