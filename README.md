# Linux Shell in C
## OSN Assignment 2
## Mihir Bani, 2019113003

### Make Instructions
In the directory containing the source code, execute:
```bash
$ make
$ ./a.out
```

## Functionality

### Prompt
  - Prompt is displayed in the same format as in BASH with username, system name and current working directory. Home directory ("~") is considered as the directory from which the shell a.out was executed. 
  - Errors while getting the current working directory (home directory), username or system name leads to termination of the shell.
  
**Input** : multiple commands can be given separated by `;`.
Extra whitespaces given in commands or arguments is ignored.  
  
**Error checking** : If any command fails to execute due to mulitple reasons, the relevant error message will be printed to the terminal by use of `perror` and in some cases without `perror`. That command will be skipped and shell will continue to run.

### Commands
  1. `pwd` - prints the absolute path of the current working directory.
  2. `echo` - prints whatever is given as argument as it is
  3. `cd` - changes the cwd to the mentioned directory, if no argument, changes to the home (~) directory.
     - arguments: `.`, `..`, `-`, `~`, any relative/absolute path to a directory
  4. `ls` - lists all the files/directories of the place passed as arguments, or if none pased the cwd.
     - flags: `-a`, `-l`, `-a -l`, `-l -a`, `-al`, `-la`. where *a* is for all, and *l* is for long.
     - Supports mulitple directories as arguments. If mulitple directories are given along with flags, the flags apply to all directories.
  5. `repeat` - takes an argument *num command*, and executes the mentioned command *num* times.
  6. `history`
     - by default prints the last 10 typed commands of shell, or if lesser than 10 available, then print them.
     - with flag *num*. Prints last *num* commands from history. *1 < num < 21*.
     - Maximum of 20 commands are stored in the history.
     - On closing the shell by using `CTRL+D` or `exit` or `quit`, the history is saved to a file `.shell_history` in the home directory. History is loaded from the same file for other shell sessions.
     - If the file `.shell_history` is not formed yet, or is unreadable, it is ignored. Errors are generated if writing to this file is problematic.
     - `Up Arrow` - on pressing the up arrow, recent commands can be seen in the order newer to older.
     - `Down Arrow` - on pressing the down arrow, recent commands can be seen in the order older to newer.
  7. `pinfo` - displays the details of a process with pid passed as argument.
     - If no argument passed, the pid of the running shell is considered. 
8. Other commands from Bash - executed with the help of `execvp` command. 
### Processes 
- **Foreground process** - execution of process that happen in the foreground. These take away control from shell and must be finished/terminated to regain control of the shell.
- **Background process** - any command passed with `&` flag is treated as background process. The user can use the shell normally without waiting for completion of command. The pid of child process is printed to terminal when command is executed. 
- **Finishing of Background process** - a message is printed whenever a background process is completed/terminated, expressing whether is was normal or abnormal termination. 

## File Structure
| Files | Functionality |
|---|---|
| cd.c | `cd` command |
| echo.c | `echo` command |
| history.c | `history` command |
| ls.c | `ls` command |
| pinfo.c | `pinfo` command |
| repeat.c | `repeat` command |
| pwd.c | `pwd` command |
| shell_commands.c | Executing foreground/background processes, and the commands which were not implemented exclusively for this shell |
| commands.h | Header file for declarations of all the commands related .c files |
| prompt.c | Prompt functionality in shell |
| utils.c | Various utility functions |
| utils.h | Header File for declaring functions in utils.c |
| main.c | The main file containing the `main` function for running the shell |
| headers.h | Header file for declaring all global variables and including other .h files |
| makefile | make instructions for compiling the code |