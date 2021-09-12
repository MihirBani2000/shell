#include "../headers.h"
#include "../commands/commands.h"
#include "utils.h"

void reset_commands_array()
{
    for (int i = 0; i < BIG_SIZE; i++)
    {
        if (all_commands[i])
            free(all_commands[i]);
        if (command[i])
            free(command[i]);

        all_commands[i] = (char *)malloc(BIG_SIZE * sizeof(char));
        command[i] = (char *)malloc(BIG_SIZE * sizeof(char));
    }
}

void tokenize_inputs(char *input_string)
{
    char *delim = ";";
    char *temp_in = strtok(input_string, delim);
    int cnt = 0;
    while (temp_in != NULL)
    {
        // printf("inside tokenize_inputs %s \n", temp_in);
        all_commands[cnt] = (char *)malloc(BIG_SIZE * sizeof(char));
        strcpy(all_commands[cnt++], temp_in);
        temp_in = strtok(NULL, delim);
    }
    num_commands = cnt;
    return;
}

int tokenize_command(int counter)
{
    for (int i = 0; i < BIG_SIZE; i++)
    {
        if (command[i])
            free(command[i]);
        command[i] = (char *)malloc(BIG_SIZE * sizeof(char));
    }

    char *delim = " \r\t\n";
    char *input_command = (char *)malloc(BIG_SIZE * sizeof(char));
    strcpy(input_command, all_commands[counter]);

    char *temp_in = strtok(input_command, delim);
    int cnt = 0;
    while (temp_in != NULL)
    {
        // printf("inside tokenize_command %s \n", temp_in);
        command[cnt] = (char *)malloc(BIG_SIZE * sizeof(char));
        strcpy(command[cnt++], temp_in);
        temp_in = strtok(NULL, delim);
    }
    num_args = cnt;
    free(input_command);
    return num_args;
}

void execute()
{
    int exec_status = 0;
    if (!strcmp(command[0], "echo"))
    {
        echo();
    }
    else if (!strcmp(command[0], "pwd"))
    {
        pwd();
    }
    else if (!strcmp(command[0], "cd"))
    {
        cd();
    }
    else if (!strcmp(command[0], "ls"))
    {
        ls();
    }
    else if (!strcmp(command[0], "quit") || !strcmp(command[0], "exit"))
    {
        exit(EXIT_SUCCESS);
    }
    else if (!strcmp(command[0], "repeat"))
    {
        repeat();
    }
    else if (!strcmp(command[0], "pinfo"))
    {
        pinfo();
    }
    // else if (!strcmp(command[0], "history"))
    // {
    //     int k = get_count();
    //     print_history(k);
    // }
    else
    {
        int bg_flag = 0;
        char temp1 = command[num_args - 1][0];
        char temp2 = command[num_args - 1][strlen(command[num_args - 1]) - 1];
        if (temp1 == '&' || temp2 == '&')
            bg_flag = 1;
        shell_commands(bg_flag);
    }
}