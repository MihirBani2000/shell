#include "../headers.h"
#include "utils.h"
#include "../commands/commands.h"

void die(const char *s)
{
    // perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr inside disableRawMode");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be
 * written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
 */
void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr inside enableRawMode");
}

/**
 * stdout and stdin are buffered we disable buffering on that
 * After entering in raw mode we read characters one by one
 * Up arrow keys and down arrow keys are represented by 3 byte escape codes
 * starting with ascii number 27 i.e. ESC key
 * This way we interpret arrow keys
 * Tabs are usually handled by the term, but here we are simulating tabs for the sake of simplicity
 * Backspace move the cursor one control character to the left
 * @return
 */

void take_input_with_arrow(char *input_commands)
{
    char c;
    int pt = 0, his_cmd_pt = 0; // for history up arrow
    enableRawMode();

    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (iscntrl(c))
        {
            if (c == 10)
                break;
            else if (c == 27)
            {
                char buf[3];
                buf[2] = 0;
                if (read(STDIN_FILENO, buf, 2) == 2)
                { // length of escape code
                    if (buf[1] == 'A')
                    // up arrow
                    {
                        char *history_command = malloc(BIG_SIZE * sizeof(char));
                        his_cmd_pt++;

                        printf("\r\33[2K");
                        printf("\r");
                        show_prompt();

                        get_history_at_index(&his_cmd_pt, history_command);
                        strcpy(input_commands, history_command);
                        pt = strlen(history_command);
                        input_commands[pt] = '\0';
                        printf("%s", history_command);
                        free(history_command);
                    }
                    else if (buf[1] == 'B')
                    // down arrow
                    {
                        char *history_command = malloc(BIG_SIZE * sizeof(char));
                        his_cmd_pt--;

                        printf("\r\33[2K");
                        printf("\r");
                        show_prompt();

                        get_history_at_index(&his_cmd_pt, history_command);
                        strcpy(input_commands, history_command);
                        pt = strlen(history_command);
                        input_commands[pt] = '\0';
                        printf("%s", history_command);
                        free(history_command);
                    }
                }
            }
            else if (c == 127)
            { // backspace
                if (pt > 0)
                {
                    if (input_commands[pt - 1] == 9)
                    {
                        for (int i = 0; i < 7; i++)
                        {
                            printf("\b");
                        }
                    }
                    input_commands[--pt] = '\0';
                    printf("\b \b");
                }
            }
            else if (c == 9)
            { // TAB character
                input_commands[pt++] = c;
                for (int i = 0; i < 8; i++)
                { // TABS should be 8 spaces
                    printf(" ");
                }
            }
            else if (c == 4)
            {
                // Ctrl + D, End of Transmission
                custom_exit(EXIT_SUCCESS);
            }
            else
            {
                printf("%d\n", c);
            }
        }
        else
        {
            input_commands[pt++] = c;
            printf("%c", c);
        }
    }
    disableRawMode();
}