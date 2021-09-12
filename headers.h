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
#include <fcntl.h>

#define BIG_SIZE 1005
#define SMALL_SIZE 255
// #define MAX_BACKGROUND_JOBS 100
// #define NUM_OF_HISTORY 20
// #define DEF_HISTORY_SHOW 10

char *HOME;
char *PROMPT;
char *LAST_WD;
char *CWD;
char *all_commands[BIG_SIZE];
char *command[BIG_SIZE];
int num_args;
int num_commands;
pid_t SHELL_ID;

#endif