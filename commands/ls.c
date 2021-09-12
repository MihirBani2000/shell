#include "../headers.h"
#include "commands.h"

void get_flags(int *flag)
{
    // stores -1 in flag[0] for no flag
    // stores 0 in flag[0] for no flag
    // stores 1 in flag[0] for `-a`
    // stores 2 in flag[0] for `-l`
    // stores 3 in flag[0] for `-al`, `-la`, `-a -l` or `-l -a`
    int a_flag = 0, l_flag = 0, c_flag = 0;
    int idx = num_args;
    while (--idx)
    {
        if (command[idx][0] == '-')
        {
            if (strlen(command[idx]) == 2)
            {
                if (!strcmp(command[idx], "-a"))
                    a_flag = 1;
                else if (!strcmp(command[idx], "-l"))
                    l_flag = 2;
            }
            else if (strlen(command[idx]) == 3)
            {
                if (!strcmp(command[idx], "-al") || !strcmp(command[idx], "-la"))
                {
                    a_flag = 1;
                    l_flag = 2;
                }
            }
            // for (int i = 1; i < strlen(command[idx]); i++)
            // {
            //     if (command[idx][i] == 'a')
            //         a_flag = 1;
            //     if (command[idx][i] == 'l')
            //         l_flag = 2;
            // }
        }
        else
            c_flag++;
    }
    if (!(a_flag + l_flag + c_flag) && num_args > 1)
    {
        flag[0] = -1;
        flag[1] = -1;
        return;
    }

    flag[0] = a_flag + l_flag;
    flag[1] = c_flag;
    return;
}

void set_perms(char *filename, char *perms)
{
    char buffer[200];
    struct stat fs;
    if (stat(filename, &fs) < 0)
    {
        sprintf(buffer, "%s:", filename);
        write(2, buffer, strlen(buffer));
        perror("Error reading permissions");
        return;
    }
    perms[0] = (fs.st_mode & S_IRUSR) ? 'r' : '-';
    perms[1] = (fs.st_mode & S_IWUSR) ? 'w' : '-';
    perms[2] = (fs.st_mode & S_IXUSR) ? 'x' : '-';
    perms[3] = (fs.st_mode & S_IRGRP) ? 'r' : '-';
    perms[4] = (fs.st_mode & S_IWGRP) ? 'w' : '-';
    perms[5] = (fs.st_mode & S_IXGRP) ? 'x' : '-';
    perms[6] = (fs.st_mode & S_IROTH) ? 'r' : '-';
    perms[7] = (fs.st_mode & S_IWOTH) ? 'w' : '-';
    perms[8] = (fs.st_mode & S_IXOTH) ? 'x' : '-';
    perms[9] = '\0';
}

void print_ls(char *path, int sp_flag)
{
    // printf("inside print_ls: %s\n", path);
    // printf("inside print_ls: %d\n", sp_flag);
    char perms[10];
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("ls: Error opening directory");
        return;
    }
    struct dirent *file;
    char *file_address = (char *)malloc(BIG_SIZE * sizeof(char));
    while ((file = readdir(dir)) != NULL)
    {
        if (((sp_flag != 1) || (sp_flag != 3)) && (file->d_name[0] == '.'))
        {
            // without `a`
            continue;
        }

        strcpy(file_address, path);
        strcat(file_address, "/");
        strcat(file_address, file->d_name);

        struct stat file_st;
        set_perms(file_address, perms);

        if (sp_flag < 2)
        {
            // without `l`
            if ((stat(file_address, &file_st) == 0) && S_ISDIR(file_st.st_mode))
                printf("\033[1;34m%s\033[0m\t", file->d_name);
            else
                printf("%s\t", file->d_name);
        }
        else
        {
            // with `l`
        }
    }

    free(file_address);
    if (sp_flag < 2)
        printf("\n");

    closedir(dir);
}

void ls()
{
    // get flags for `a` and `l`
    int sp_flag[2];
    get_flags(sp_flag);
    printf("inside ls: %d - %d\n", sp_flag[0], sp_flag[1]);

    if (sp_flag[0] < 0)
    {
        printf("ls: invalid flags\n");
        return;
    }

    int numarg = num_args;
    // int _flag = 0;
    if (sp_flag[1] == 0)
    {
        print_ls(".", sp_flag[0]);
        return;
    }

    while (--numarg && sp_flag[1])
    {
        if (command[numarg][0] != '-')
        {
            // for cases `.` `..` `~` and other directories
            sp_flag[1]--;
            char *path = (char *)malloc(BIG_SIZE * sizeof(char));
            if (command[numarg][0] == '~')
            {
                strcpy(path, HOME);
                strcat(path, command[numarg] + 1);
            }
            else
            {
                strcpy(path, command[numarg]);
            }
            printf("\033[1;36m%s\033[0m:\n", path);
            print_ls(path, sp_flag[0]);
            printf("\n");
            free(path);
        }
    }

    return;
}