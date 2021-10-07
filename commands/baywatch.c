#include "../headers.h"
#include "../utils/utils.h"
#include "commands.h"

int check_baywatch_flags(int *seconds)
{
    // returns < 0 for errors
    // return 1 for interrup
    // return 2 for newborn
    // return 3 for dirty
    int flag = -1;
    int c_flag = 0;
    int n_flag = 0;

    int i = 0;
    while (i < num_args)
    {
        if (!strcmp(command[i], "-n"))
        {
            n_flag++;
            if (command[i + 1] != NULL)
                *seconds = atoi(command[++i]);
        }
        else if (!strcmp(command[i], "interrupt"))
        {
            c_flag++;
            flag = 1;
        }
        else if (!strcmp(command[i], "newborn"))
        {
            c_flag++;
            flag = 2;
        }
        else if (!strcmp(command[i], "dirty"))
        {
            c_flag++;
            flag = 3;
        }
        i++;
    }

    if ((c_flag != 1) && (n_flag != 1))
    {
        fprintf(stderr, "baywatch: Invalid arguments\n");
        return -1;
    }
    if ((seconds <= 0))
    {
        fprintf(stderr, "baywatch: Invalid arguments\n");
        return -1;
    }

    return flag;
}

void interrupt(int seconds)
{
    FILE *f_int;
    f_int = fopen("/proc/interrupts", "r");
    if (f_int == NULL)
    {
        char *err_buf = (char *)malloc(SMALL_SIZE * sizeof(char));
        sprintf(err_buf, "baywatch: eror in opening /proc/interrupts file");
        perror(err_buf);
        free(err_buf);
        return;
    }
    char file_data[BIG_SIZE];
    fgets(file_data, BIG_SIZE, f_int);
    printf("%s\n", file_data);

    while (1)
    {
        f_int = fopen("/proc/interrupts", "r");
        if (f_int == NULL)
        {
            char *err_buf = (char *)malloc(SMALL_SIZE * sizeof(char));
            sprintf(err_buf, "baywatch: eror in opening /proc/interrupts file");
            perror(err_buf);
            free(err_buf);
            return;
        }

        char file_data[BIG_SIZE];
        for (int i = 0; i < 3; i++)
        {
            fgets(file_data, BIG_SIZE, f_int);
        }

        int start_flag = 1;
        int end_flag = 0;
        int len_file_data = strlen(file_data);

        for (int j = 0; j < len_file_data; j++)
        {
            if (start_flag == 1)
            {
                printf(" ");
                if (file_data[j] == ':')
                    start_flag = 0;
            }
            else if (file_data[j] == 'I')
            {
                end_flag = 1;
                printf("\n");
                break;
            }
            else
            {
                printf("%c", file_data[j]);
            }
            // printf("abra %d %c\n", j, file_data[j]);
        }
        // printf("%s\n", file_data);
        fclose(f_int);
        sleep(seconds);
    }

    return;
}

void newborn(int seconds)
{

    while (1)
    {
        FILE *f_loadavg;
        f_loadavg = fopen("/proc/loadavg", "r");
        if (f_loadavg == NULL)
        {
            char *err_buf = (char *)malloc(SMALL_SIZE * sizeof(char));
            sprintf(err_buf, "baywatch: eror in opening /proc/loadavg file");
            perror(err_buf);
            free(err_buf);
            return;
        }

        char *line = malloc(BIG_SIZE * sizeof(char));
        fgets(line, BIG_SIZE, f_loadavg);

        char *value = strtok(line, " ");
        // for (int i = 0; i < 4; i++)
        value = strtok(NULL, " ");
        value = strtok(NULL, " ");
        value = strtok(NULL, " ");
        value = strtok(NULL, " ");

        fclose(f_loadavg);

        printf("%s", value);
        sleep(seconds);
    }

    return;
}

void dirty(int seconds)
{
    while (1)
    {
        FILE *f_meminfo;
        char *file_buffer = malloc(BIG_SIZE * sizeof(char));
        char *memory = malloc(SMALL_SIZE * sizeof(char));
        char *unit = malloc(SMALL_SIZE * sizeof(char));

        f_meminfo = fopen("/proc/meminfo", "r");
        if (f_meminfo == NULL)
        {
            char *err_buf = (char *)malloc(SMALL_SIZE * sizeof(char));
            sprintf(err_buf, "baywatch: eror in opening /proc/meminfo file");
            perror(err_buf);
            free(err_buf);
            return;
        }

        while (fgets(file_buffer, BIG_SIZE, f_meminfo))
        {
            char *token = strtok(file_buffer, " \t\r");
            if (strcmp(token, "Dirty:") == 0)
            {
                token = strtok(NULL, " \r\t");
                strcpy(memory, token);
                token = strtok(NULL, " \r\t");
                strcpy(unit, token);
                break;
            }
        }
        printf("%s %s\n", memory, unit);
        fclose(f_meminfo);
        sleep(seconds);
    }

    return;
}

void baywatch()
{
    // baywatch -n <seconds> <command>
    if (num_args != 4)
    {
        fprintf(stderr, "baywatch: invalid arguments\n");
        return;
    }

    int n = -1;

    int flag = check_baywatch_flags(&n);
    if (flag > 0)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("baywatch: error while forking");
            return;
        }
        else if (!pid)
        {
            // child
            if (flag == 1)
                interrupt(n);
            else if (flag == 2)
                newborn(n);
            else if (flag == 3)
                dirty(n);

            exit(1);
        }
        else
        {
            // parent

            char ch;
            int break_flag = 1;
            setbuf(stdout, NULL);
            enableRawMode();
            while (break_flag)
            {
                while (read(STDIN_FILENO, &ch, 1) == 1)
                {
                    if (ch == 113)
                    {
                        break_flag = 0;
                        kill(pid, SIGKILL);
                        break;
                    }
                }
            }
            disableRawMode();

            int status;
            waitpid(pid, &status, WUNTRACED);
        }
    }
    else
    {
        fprintf(stderr, "baywatch: invalid input\n");
    }

    return;
}