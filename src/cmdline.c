#include <stddef.h>
#include <stdlib.h>
#include "ghostsh.h"

gstsh_command_line_t* gstsh_command_line_new(void)
{
    gstsh_command_line_t* cmd = malloc(sizeof(gstsh_command_line_t));
    cmd->argc = 0;
    cmd->argv = NULL;
    cmd->pipe_to = NULL;
    return cmd;
}

void gstsh_command_line_clear(gstsh_command_line_t* cmd)
{
    cmd->argc = 0;
    if (cmd->argv != NULL)
    {
        free(cmd->argv);
        cmd->argv = NULL;
    }
    if (cmd->pipe_to != NULL)
    {
        gstsh_command_line_free(cmd->pipe_to);
        cmd->pipe_to = NULL;
    }
}

void gstsh_command_line_free(gstsh_command_line_t* cmd)
{
    gstsh_command_line_clear(cmd);
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