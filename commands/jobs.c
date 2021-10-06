#include "../headers.h"
#include "commands.h"

void lex_sort(int *arr)
{ // bubble sort
    int temp = 0;
    for (int i = 1; i < num_bgjobs; i++)
    {
        for (int j = 1; j <= num_bgjobs - i; j++)
        {
            if (strcmp(bgjobs[arr[j]].name, bgjobs[arr[j + 1]].name) > 0)
            {
                // a < b
                temp = arr[j + 1];
                arr[j + 1] = arr[j];
                arr[j] = temp;
            }
        }
    }
    return;
}

void print_jobs(int flag)
{
    // flag == 0, for no flags
    // flag == 1, for stopped -s flag
    // flag == 2, for running -r flag

    char *proc_file_path = malloc(BIG_SIZE * sizeof(char));

    int temp_bgjobs[num_bgjobs + 1];
    for (int cur = 0; cur <= num_bgjobs; cur++)
    {
        temp_bgjobs[cur] = cur;
    }

    // sorting the entries by alphabetical order of command names
    lex_sort(temp_bgjobs);
    // printf("HIHI\n");
    // for (int i = 1; i <= num_bgjobs; i++)
    // {
    //     printf("%d - ", temp_bgjobs[i]);
    //     printf("%s\n", bgjobs[temp_bgjobs[i]].name);
    // }

    for (int cur = 1; cur < num_bgjobs + 1; cur++)
    {
        int idx = temp_bgjobs[cur];
        int proc_pid = bgjobs[idx].pid;
        char *file_buffer = malloc(BIG_SIZE * sizeof(char));
        char *p_status = malloc(SMALL_SIZE * sizeof(char));
        char *output_buffer = malloc(BIG_SIZE * sizeof(char));

        FILE *proc_file;

        sprintf(proc_file_path, "/proc/%d/status", proc_pid);
        proc_file = fopen(proc_file_path, "r");
        if (proc_file == NULL)
        {
            // char *err_buf = malloc(SMALL_SIZE * sizeof(char));
            // sprintf(err_buf, "Process with pid: %d doesn't exist", proc_pid);
            // perror(err_buf);
            // free(err_buf);
            continue;
        }

        while (fgets(file_buffer, BIG_SIZE, proc_file))
        {
            // get the process status from status file
            char *token = strtok(file_buffer, " \t\r");
            if (strcmp(token, "State:") == 0)
            {
                token = strtok(NULL, " \t");
                strcpy(p_status, token);
                break;
            }
        }
        int T_flag = !strcmp(p_status, "T");
        if (flag == 1)
        {
            // only stopped processes
            if (T_flag)
            {
                printf("[%d] Stopped %s [%d]\n", idx, bgjobs[idx].name, proc_pid);
            }
        }
        else if (flag == 2)
        {
            // only running processes
            if (!T_flag)
            {
                printf("[%d] Running %s [%d]\n", idx, bgjobs[idx].name, proc_pid);
            }
        }
        else
        {
            // all processes
            if (T_flag)
            {
                printf("[%d] Stopped %s [%d]\n", idx, bgjobs[idx].name, proc_pid);
            }
            else
            {
                printf("[%d] Running %s [%d]\n", idx, bgjobs[idx].name, proc_pid);
            }
        }
        free(output_buffer);
        free(file_buffer);
        free(p_status);
    }
    return;
}

void jobs()
{
    if ((num_args < 1) || (num_args > 2))
    {
        fprintf(stderr, "jobs: Invalid arguments\n");
        return;
    }

    if (num_args == 2)
    {
        // flags present
        if (!strcmp(command[1], "-s"))
        {
            // only stopped ones
            if (num_bgjobs <= 0)
                printf("No jobs\n");
            else
                print_jobs(1);
            return;
        }
        else if (!strcmp(command[1], "-r"))
        {
            // only running ones
            if (num_bgjobs <= 0)
                printf("No jobs\n");
            else
                print_jobs(2);
            return;
        }
        else
        {
            fprintf(stderr, "jobs: Invalid arguments\n");
            return;
        }
    }

    if (num_bgjobs <= 0)
        printf("No jobs\n");
    else
        print_jobs(0);
    return;
}

// i can take all the names and the index and store it in some data structure
// perform the sort by names on this
// iterate thru this data structure, and get the details of the process by index
// and then using pid, from the status file
// print this sequentially