#include "../headers.h"
#include "../utils/utils.h"
#include "commands.h"

void reset_history_buffer()
{
    for (int i = 0; i < MAX_HISTORY; i++)
    {
        if (history[i])
            free(history[i]);
    }
    hist_count = 0;
    return;
}

void load_history_from_file()
{
    for (int i = 0; i < MAX_HISTORY; i++)
    {
        history[i] = (char *)malloc(BIG_SIZE * sizeof(char));
    }

    FILE *hfile = fopen(history_path, "r");
    hist_count = 0;
    if (hfile == NULL)
    {
        // perror("history: unable to open/create file .shell_history");
        return;
    }
    while (fgets(history[hist_count], BIG_SIZE, hfile) != NULL)
    {
        hist_count++;
        if (hist_count > MAX_HISTORY)
            break;
    }

    fclose(hfile);
    return;
}

void save_history_to_file()
{
    FILE *hfile = fopen(history_path, "w+");
    if (hfile == NULL)
    {
        perror("history: unable to open file .shell_history");
        return;
    }

    int temp = 0;
    while (temp < hist_count)
    {
        fputs(history[temp], hfile);
        temp++;
    }
    hist_count = 0;
    fclose(hfile);
    reset_history_buffer();
    return;
}

void add_to_history_buffer(char *line)
{
    if (hist_count && strcmp(history[hist_count - 1], line) == 0)
        return;
    if (hist_count == MAX_HISTORY)
    {
        for (int i = 1; i < hist_count; i++)
        {
            strcpy(history[i - 1], history[i]);
        }
        strcpy(history[hist_count - 1], line);
    }
    else if (hist_count < MAX_HISTORY)
    {
        strcpy(history[hist_count], line);
        hist_count++;
    }

    return;
}

void print_history()
{
    if (num_args > 2)
    {
        fprintf(stderr, "history: invalid arguments\n");
        return;
    }

    int print_num = 10;
    if (num_args == 2)
        print_num = atoi(command[1]);

    if ((print_num < 1) || (print_num > 20))
    {
        fprintf(stderr, "history: invalid arguments\n");
        return;
    }

    int start = 0;
    if (hist_count >= print_num)
        start = hist_count - print_num;

    for (int i = start; i < hist_count; i++)
    {
        printf("%s", history[i]);
    }

    return;
}

void get_history_at_index(int *ind, char *input_command)
{
    if (hist_count == 0)
    {
        strcpy(input_command, "\0");
        return;
    }
    if (*ind <= 0)
    {
        *ind = 0;
        strcpy(input_command, "\0");
        return;
    }

    int counter = hist_count - *ind;
    if (counter > 0)
    {
        strcpy(input_command, history[counter]);
    }
    else
    {
        *ind = hist_count;
        strcpy(input_command, history[0]);
    }

    int len = strlen(input_command);
    if (input_command[len - 1] == '\n')
        input_command[len - 1] = '\0';

    return;
}