#ifndef UTILS
#define UTILS

void show_prompt();
void reset_commands_array();
void tokenize_inputs(char *input_string);
int tokenize_command(int counter);
void handle_child_finish();
void add_bg_proc(pid_t pid, char **proc_details);
void custom_exit(int flag);
void execute();

#endif