#include "../headers.h"
#include "../commands/commands.h"
#include "utils.h"

int check_pipe(int counter)
{
    // returns 0 for no piping else 1
    if (strstr(all_commands[counter], "|") == NULL)
        return 0;
    return 1;
}

int tokenize_piped(char *input_string)
{
    for (int i = 0; i < BIG_SIZE; i++)
    {
        if (pipe_commands[i])
            free(pipe_commands[i]);
        pipe_commands[i] = (char *)malloc(BIG_SIZE * sizeof(char));
    }
    char *delim = "|";
    char *temp_in = strtok(input_string, delim);
    int cnt = 0;

    while (temp_in != NULL)
    {
        // printf("inside tokenize_pipe %s \n", temp_in);
        pipe_commands[cnt] = malloc(BIG_SIZE * sizeof(char));
        strcpy(pipe_commands[cnt++], temp_in);
        temp_in = strtok(NULL, delim);
    }
    pipe_commands[cnt] = NULL;
    return cnt;
}

void piped_execute(char *input_command)
{

    int filedes[2]; // filedes[0] has read end, filedes[1] has write end
    int in_fd_prev = dup(STDIN_FILENO);
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);
    int num_pipes = tokenize_piped(input_command);

    for (int i = 0; i < num_pipes; i++)
    {
        if (pipe(filedes) != 0)
        {
            perror("pipe: error in making pipe");
            return;
        }

        pid_t pid = fork();
        if (pid < 0)
        {
            perror("pipe: error while forking");
            return;
        }
        else if (!pid)
        { // child process

            dup2(in_fd_prev, STDIN_FILENO); // setting the in_file from prev command
            if (close(filedes[0]) < 0)      // first closing the read end
                perror("error in closing piping file");

            if (i < num_pipes - 1)
                dup2(filedes[1], STDOUT_FILENO);

            if (tokenize_command(pipe_commands[i]) > 0)
            {
                // printf("\n");
                execute();
            }

            if (close(filedes[1]) < 0)
                perror("error in closing piping file");
            exit(1);
        }
        else
        { //parent

            // in_fd_prev = filedes[0];
            dup2(filedes[0], in_fd_prev); // saving for next command

            if (close(filedes[0]) < 0)
                perror("error in closing piping file");

            wait(NULL); // waiting for child to finish

            if (close(filedes[1]) < 0)
                perror("error in closing piping file");
        }
    }

    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stdin, STDIN_FILENO);
    fflush(stdout);
    fflush(stdin);
    return;
}
