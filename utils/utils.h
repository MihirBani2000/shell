#ifndef UTILS
#define UTILS

void show_prompt();
void reset_commands_array();
void tokenize_inputs(char *input_string);
int tokenize_command(int counter);
void execute(char *input_command);

#endif