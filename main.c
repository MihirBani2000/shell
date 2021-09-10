#include "headers.h"
#include "utils/utils.h"
#include "commands/commands.h"

int initialize_shell()
{
    if (HOME)
        free(HOME);
    HOME = (char *)malloc(BIG_SIZE * sizeof(char));
    HOME = getcwd(HOME, BIG_SIZE);
    if (HOME == NULL)
    {
        perror("Error retreiving Current Working Directory");
        return 1;
    }
    LAST_WD = (char *)malloc(BIG_SIZE * sizeof(char));
    CWD = (char *)malloc(BIG_SIZE * sizeof(char));
    strcpy(LAST_WD, HOME);
    strcpy(CWD, HOME);
    SHELL_ID = getpid();
    return 0;
}

int main()
{

    printf("Starting Shell...\n");
    if (initialize_shell() != 0)
    {
        printf("Error in starting Shell.\n Exiting ...\n");
        return 1;
    }
    while (1)
    {
        show_prompt();
        reset_commands_array();

        char *input_commands = (char *)malloc(BIG_SIZE * sizeof(char));
        char tempchar;
        scanf("%[^\n]s", input_commands);
        scanf("%c", &tempchar);
        // printf("input %s\n", input_commands);

        if (strcmp(input_commands, "\n") == 0)
            continue; // continue if press entered

        tokenize_inputs(input_commands);
        free(input_commands);

        int counter = 0;
        while (counter < num_commands)
        {
            if (tokenize_command(counter) >= 0)
            {
                // execute(command);
            }
            counter++;
        }
    }

    return 0;
}