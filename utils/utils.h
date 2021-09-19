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
void die(const char *s);
void disableRawMode();
void enableRawMode();
void take_input_with_arrow(char *input_commands);

#endif