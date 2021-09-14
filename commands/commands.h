#ifndef COMMMANDS
#define COMMMANDS

void cd();
void echo();
void ls();
void pinfo();
void pwd();
void repeat();
void shell_commands(int bg);

// history
void reset_history_buffer();
void load_history_from_file();
void save_history_to_file();
void add_to_history_buffer(char *line);
void print_history();

#endif