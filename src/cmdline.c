#include <stddef.h>
#include <stdlib.h>
#include "ghostsh.h"

gs_command_line_t* gs_command_line_new(void)
{
    gs_command_line_t* cmd = malloc(sizeof(gs_command_line_t));
    cmd->argc = 0;
    cmd->argv = NULL;
    cmd->pipe_to = NULL;
    return cmd;
}

void gs_command_line_clear(gs_command_line_t* cmd)
{
    cmd->argc = 0;
    if (cmd->argv != NULL)
    {
        free(cmd->argv);
        cmd->argv = NULL;
    }
    if (cmd->pipe_to != NULL)
    {
        gs_command_line_free(cmd->pipe_to);
        cmd->pipe_to = NULL;
    }
}

void gs_command_line_free(gs_command_line_t* cmd)
{
    gs_command_line_clear(cmd);
    free(cmd);
}

void gs_command_line_set_argv(gs_command_line_t* cmd, int argc, gs_charp_list_t* argv)
{
    cmd->argc = argc;
    cmd->argv = malloc(sizeof(char*) * (argc + 1));
    for (int i = 0; i < argc; i++)
    {
        cmd->argv[i] = argv->data;
        argv = argv->next;
    }
    cmd->argv[argc] = NULL;
}
