#include "../headers.h"
#include "../commands/commands.h"
#include "utils.h"

int check_redirect(int counter)
{
    // returns 0 for no redirection else 1
    if ((strstr(all_commands[counter], ">") == NULL) && (strstr(all_commands[counter], "<") == NULL))
        return 0;
    return 1;
}

void redirected_execute(int counter)
{
}
