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
    command[num_args] = NULL;
    free(input_command);
    return num_args;
}

void handle_child_finish()
{
    int status;
    char *temp_name = (char *)malloc(BIG_SIZE);
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if (pid <= 0)
        return;
    // some child process finished
    for (int i = 0; i < num_bgjobs; i++)
    {
        if (bgjobs[i].pid == pid)
        { // this job is now removed
            num_bgjobs--;
            strcpy(temp_name, bgjobs[i].name);
            for (int j = i + 1; j <= num_bgjobs; j++)
            {
                bgjobs[j - 1].pid = bgjobs[j].pid;
                strcpy(bgjobs[j - 1].name, bgjobs[j].name);
            }
            break;
        }
    }
    if (WIFEXITED(status) && !WEXITSTATUS(status))
        fprintf(stderr, "\n%s with PID %d exited normally.\n", temp_name, pid);
    else
        fprintf(stderr, "\n%s with PID %d exited abnormally.\n", temp_name, pid);

    show_prompt();
    free(temp_name);
    fflush(stdout);
}

void add_bg_proc(pid_t pid, char **proc_command)
{
    int idx = 0;
    strcpy(bgjobs[num_bgjobs].name, proc_command[0]);
    bgjobs[num_bgjobs].pid = pid;

    char *temp = (char *)malloc(SMALL_SIZE);
    while (proc_command[++idx] != NULL)
    {
        sprintf(temp, " %s", proc_command[idx]);
        strcat(bgjobs[num_bgjobs].name, temp);
    }
    free(temp);
    ++num_bgjobs;
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
        int bg_flag = 0, len = strlen(command[num_args - 1]);
        char temp1 = command[num_args - 1][len - 1]; // xeyes&
        char temp2 = command[num_args - 1][0];       // xeyes &
        if (temp2 == '&')
        {
            bg_flag = 1;
            --num_args;
        }
        else if (temp1 == '&')
        {
            bg_flag = 1;
            command[num_args - 1][len - 1] = '\0';
        }
        shell_commands(bg_flag);
    }
}