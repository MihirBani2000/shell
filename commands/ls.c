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
    return;
}

void ls_for_file(char *file_address, int sp_flag)
{
    char *details = (char *)malloc(BIG_SIZE * sizeof(char));
    char *fname = (char *)malloc(SMALL_SIZE * sizeof(char));
    long total = 0;
    struct stat file_st;
    char perms[10];

    if (stat(file_address, &file_st) != 0)
    {
        perror("ls: error accessing file");
        return;
    }

    // get file name
    int pos = strlen(file_address);
    while (pos >= 0 && file_address[pos] != '/')
    {
        pos--;
    }
    pos++;
    strcpy(fname, file_address + pos);

    if (sp_flag < 2)
    {
        // without `l`
        printf("\033[0m%s\n", fname);
    }
    else
    {
        // with `l`
        char *temp_buffer = (char *)malloc(BIG_SIZE * sizeof(char));
        set_perms(file_address, perms);
        struct passwd *pw = getpwuid(file_st.st_uid);
        struct group *gr = getgrgid(file_st.st_gid);
        char *time = (char *)malloc(SMALL_SIZE * sizeof(char));
        total += file_st.st_blocks;

        // perms
        strcpy(details, "-");
        sprintf(temp_buffer, "%s  ", perms);
        strcat(details, temp_buffer);

        // links, owner, group
        sprintf(temp_buffer, "%ld\t", file_st.st_nlink);
        strcat(details, temp_buffer);
        if ((pw != 0) && (gr != 0))
        {
            sprintf(temp_buffer, "%s\t", pw->pw_name);
            strcat(details, temp_buffer);
            sprintf(temp_buffer, "%s\t", gr->gr_name);
            strcat(details, temp_buffer);
        }
        else
        {
            printf("ls: error in retreiving owner/group of file: %s\n", fname);
            return;
        }

        // size
        sprintf(temp_buffer, "%ld\t", file_st.st_size);
        strcat(details, temp_buffer);
        // date and time
        strftime(time, 50, "%b %d %H:%M", localtime(&file_st.st_mtime));
        sprintf(temp_buffer, "%s\t", time);
        strcat(details, temp_buffer);

        // for name
        sprintf(temp_buffer, "%s", fname);

        strcat(details, temp_buffer);
        free(temp_buffer);
        free(time);
    }

    if (sp_flag > 1)
    { // printing the list
        printf("total %ld\n", total / 2);
        printf("%s\n", details);
    }

    free(fname);
    free(details);

    if (sp_flag < 2)
        printf("\n");

    return;
}

void print_ls(char *path, int sp_flag)
{
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
    {
        perror("ls: invalid argument");
        return;
    }

    DIR *dir = opendir(path);
    if (errno == ENOTDIR)
    {
        ls_for_file(path, sp_flag);
        // printf("hi ai ma dal \n");
        return;
    }
    else if (dir == NULL)
    {
        perror("ls: Error opening directory");
        return;
    }

    char *details[BIG_SIZE];
    for (int i = 0; i < BIG_SIZE; i++)
    {
        details[i] = (char *)malloc(BIG_SIZE * sizeof(char));
    }
    long total = 0, num_entries = 0;

    struct dirent *file;
    char *file_address = (char *)malloc(BIG_SIZE * sizeof(char));

    while ((file = readdir(dir)) != NULL)
    {
        char *fname = (char *)malloc(SMALL_SIZE * sizeof(char));
        fname = file->d_name;
        if (((sp_flag != 1) && (sp_flag != 3)) && (fname[0] == '.'))
        {
            // without `a`
            continue;
        }

        char perms[10];
        struct stat file_st;
        strcpy(file_address, path);
        strcat(file_address, "/");
        strcat(file_address, fname);

        if (sp_flag < 2)
        {
            // without `l`
            if ((stat(file_address, &file_st) == 0) && S_ISDIR(file_st.st_mode))
                printf("\033[1;34m%s\033[0m\n", fname);
            else
                printf("\033[0m%s\n", fname);
        }
        else
        {
            // with `l`
            if (stat(file_address, &file_st) < 0)
            {
                printf("Error : %s", file_address);
                perror("ls: error in reading mentioned file/directory");
                continue;
            }
            char *temp_buffer = (char *)malloc(BIG_SIZE * sizeof(char));
            set_perms(file_address, perms);
            struct passwd *pw = getpwuid(file_st.st_uid);
            struct group *gr = getgrgid(file_st.st_gid);
            char *time = (char *)malloc(SMALL_SIZE * sizeof(char));
            total += file_st.st_blocks;

            // perms
            if (S_ISDIR(file_st.st_mode))
                strcpy(details[num_entries], "d");
            else
                strcpy(details[num_entries], "-");
            sprintf(temp_buffer, "%s  ", perms);
            strcat(details[num_entries], temp_buffer);

            // links, owner, group
            sprintf(temp_buffer, "%ld\t", file_st.st_nlink);
            strcat(details[num_entries], temp_buffer);
            if ((pw != 0) && (gr != 0))
            {
                sprintf(temp_buffer, "%s\t", pw->pw_name);
                strcat(details[num_entries], temp_buffer);
                sprintf(temp_buffer, "%s\t", gr->gr_name);
                strcat(details[num_entries], temp_buffer);
            }
            else
            {
                printf("ls: error in retreiving owner/group of file: %s\n", fname);
                continue;
            }

            // size
            sprintf(temp_buffer, "%ld\t", file_st.st_size);
            strcat(details[num_entries], temp_buffer);
            // date and time
            strftime(time, 50, "%b %d %H:%M", localtime(&file_st.st_mtime));
            sprintf(temp_buffer, "%s\t", time);
            strcat(details[num_entries], temp_buffer);

            // for name
            if (S_ISDIR(file_st.st_mode))
                sprintf(temp_buffer, "\033[1;34m%s\033[0m", fname);
            else
                sprintf(temp_buffer, "%s", fname);

            strcat(details[num_entries], temp_buffer);
            num_entries++;
            free(temp_buffer);
            free(time);
        }
        // free(fname);
    }

    if (sp_flag > 1)
    { // printing the list
        printf("total %ld\n", total / 2);
        for (int i = 0; i < num_entries; i++)
        {
            printf("%s\n", details[i]);
        }
    }

    free(file_address);
    if (sp_flag < 2)
        printf("\n");

    for (int i = 0; i < BIG_SIZE; i++)
    {
        free(details[i]);
    }
    closedir(dir);
}

void ls()
{
    // get flags for `a` and `l`
    int sp_flag[2];
    get_flags(sp_flag);
    // printf("inside ls: %d - %d\n", sp_flag[0], sp_flag[1]);

    if (sp_flag[0] < 0)
    {
        printf("ls: invalid flags\n");
        return;
    }

    int numarg = 1;
    if (sp_flag[1] == 0)
    {
        print_ls(".", sp_flag[0]);
        return;
    }

    while ((numarg < num_args) && sp_flag[1])
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
            free(path);
            // if (sp_flag[1])
            //     printf("\n");
        }
        numarg++;
    }
    return;
}