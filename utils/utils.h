#ifndef UTILS
#define UTILS

// utils.c
void show_prompt();
void reset_commands_array();
void tokenize_inputs(char *input_string);
int tokenize_command(char *input_command);
void handle_child_finish();
void add_bg_proc(pid_t pid, char **proc_details);
void custom_exit(int flag);
void execute();

// arrow.c
void die(const char *s);
void disableRawMode();
void enableRawMode();
void take_input_with_arrow(char *input_commands);

// redirect.c
int check_redirect(int counter);
void redirected_execute(int counter);

// piping.c
int check_pipe(int counter);
// int tokenize_piped(char *input_string);
void piped_execute(char *input_command);

#endif