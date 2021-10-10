#include "../headers.h"
#include "../commands/commands.h"
#include "utils.h"

int check_redirect()
{
    // returns 0 for no redirection else 1
    for (int i = 0; i < num_args; i++)
    {
        if (!strcmp(command[i], ">") || !strcmp(command[i], ">>") || !strcmp(command[i], "<"))
            return 1;
    }
    return 0;
}

int handle_input_redir()
{
    int index_redir = -1;
    int fd_in = -1;
    char *temp1 = malloc(BIG_SIZE * sizeof(char));
    char *input_file_name = malloc(BIG_SIZE * sizeof(char));
    char *input_file = malloc(BIG_SIZE * sizeof(char));

    for (int i = 0; i < num_args; i++)
    {
        if (!strcmp(command[i], "<"))
        {
            index_redir = i;
            strcpy(input_file_name, command[i + 1]);

            // check for ~
            if (input_file_name[0] == '~')
            {
                strcpy(input_file, HOME);
                strcat(input_file, input_file_name + 1);
            }
            else
            {
                strcpy(input_file, input_file_name);
            }
            // printf("file: %s", input_file);
            if (input_file == NULL || !is_valid_file(input_file))
            {
                fprintf(stderr, "redirection: Input File not found\n");
                return -1;
            }
            if ((fd_in = open(input_file, O_RDONLY)) < 0)
            {
                perror("redirection: Error opening input file. ");
                return -1;
            }
            strcpy(command[i], " ");
            strcpy(command[i + 1], " ");
        }
    }

    if (index_redir < 0)
        return 0;

    dup2(fd_in, STDIN_FILENO);
    close(fd_in);
    free(input_file);
    free(input_file_name);

    return 1;
}

int handle_output_redir()
{
    int index_redir = -1;
    int fd_out = -1;
    char *output_file_name = malloc(BIG_SIZE * sizeof(char));
    char *output_file = malloc(BIG_SIZE * sizeof(char));

    for (int i = 0; i < num_args; i++)
    {
        if (!strcmp(command[i], ">") || !strcmp(command[i], ">>"))
        {
            index_redir = i;
            strcpy(output_file_name, command[i + 1]);

            // check for ~
            if (output_file_name[0] == '~')
            {
                strcpy(output_file, HOME);
                strcat(output_file, output_file_name + 1);
            }
            else
            {
                strcpy(output_file, output_file_name);
            }
            // printf("file: %s", output_file);

            if (output_file == NULL)
            {
                fprintf(stderr, "redirection: Output File name not given\n");
                return -1;
            }
            if (!strcmp(command[i], ">>"))
            {
                fd_out = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            }
            else if (!strcmp(command[i], ">"))
            {
                // printf("> wala case\n");
                fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            if (fd_out < 0)
            {
                perror("redirection: Error opening output file. ");
                return -1;
            }
            strcpy(command[i], " ");
            strcpy(command[i + 1], " ");
        }
    }

    if (index_redir < 0)
        return 0;

    dup2(fd_out, STDOUT_FILENO);
    close(fd_out);
    free(output_file);
    free(output_file_name);

    return 1;
}

void execute_redirected_command()
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
    // command[num_args] = NULL;

    if (num_args > 0)
        execute();

    return;
}

void redirected_execute()
{
    // saving copy
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);

    // checking for redirections
    if (handle_input_redir() == -1)
    {
        // printf("in handle_input_redir\n");
        return;
    }

    if (handle_output_redir() == -1)
    {
        // printf("in handle_output_redir\n");
        return;
    }

    execute_redirected_command();

    // reverting back to original
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
}
