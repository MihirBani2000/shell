#ifndef HEADERS
#define HEADERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h> // ls
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> // struct stat
#include <signal.h>
#include <unistd.h> //getcwd
#include <pwd.h>    //getpwuid
#include <grp.h>
#include <fcntl.h>
#include <time.h>

// global parameters
#define BIG_SIZE 1005
#define SMALL_SIZE 255
#define MAX_BG_JOBS 200
// #define MAX_HISTORY 20
// #define DEF_HISTORY_SHOW 10

// global variables
pid_t SHELL_ID;
int num_args;
int num_commands;
char *HOME;
char *PROMPT;
char *LAST_WD;
char *CWD;
char *all_commands[BIG_SIZE];
char *command[BIG_SIZE];

// background jobs related
int num_bgjobs;
struct Background_Jobs
{
    char name[BIG_SIZE];
    pid_t pid;
};
struct Background_Jobs bgjobs[MAX_BG_JOBS];

// history related
// int idx_of_history;
// char history[MAX_HISTORY][BIG_SIZE];
// char history_path[BIG_SIZE];

#endif