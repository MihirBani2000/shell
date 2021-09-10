#include "../headers.h"

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
    char *delim = " \r\t\n";
    char *input_command = (char *)malloc(BIG_SIZE * sizeof(char));
    strcpy(input_command, all_commands[counter]);

    char *temp_in = strtok(input_command, delim);
    int cnt = 0;
    while (temp_in != NULL)
    {
        printf("inside tokenize_command %s \n", temp_in);
        command[cnt] = (char *)malloc(BIG_SIZE * sizeof(char));
        strcpy(command[cnt++], temp_in);
        temp_in = strtok(NULL, delim);
    }
    num_args = cnt;
    free(input_command);
    return num_args;
}

void execute(char *input_command)
{
}