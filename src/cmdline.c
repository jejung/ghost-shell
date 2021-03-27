#include <stddef.h>
#include <stdlib.h>
#include "ghostsh.h"

gstsh_command_line_t* gstsh_command_line_new(void)
{
    gstsh_command_line_t* cmd = malloc(sizeof(gstsh_command_line_t));
    gstsh_command_line_clear(cmd);
    return cmd;
}

void gstsh_command_line_clear(gstsh_command_line_t* cmd)
{
    cmd->info_flags = 0;
    cmd->program = NULL;
}

void gstsh_command_line_free(gstsh_command_line_t* cmd)
{
    free(cmd->program);
    free(cmd);
}