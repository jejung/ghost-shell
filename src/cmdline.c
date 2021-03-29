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
    cmd->argc = 0;
    cmd->argv = NULL;
}

void gstsh_command_line_free(gstsh_command_line_t* cmd)
{
    free(cmd);
}

void gstsh_command_line_set_argv(gstsh_command_line_t* cmd, int argc, gstsh_charp_list_t* argv)
{
    cmd->argc = argc;
    cmd->argv = malloc(sizeof(char*) * argc);
    for (int i = 0; i < argc; i++)
    {
        cmd->argv[i] = argv->data;
        argv = argv->next;
    }
}