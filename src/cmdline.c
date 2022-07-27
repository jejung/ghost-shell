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

void gs_command_line_add_argv(gs_command_line_t* cmd, char* argv)
{
    if (cmd->argc == 0)
    {
        cmd->argv = (char**) malloc(sizeof(char*) * 2);
    } else
    {
#ifdef __MACH__
        cmd->argv = (char**) reallocf(cmd->argv, sizeof(char*) * (cmd->argc + 2));
#else
        cmd->argv = (char**) realloc(cmd->argv, sizeof(char*) * (cmd->argc + 2));
#endif
    }
    cmd->argv[cmd->argc] = argv;
    cmd->argv[++cmd->argc] = NULL;
}
