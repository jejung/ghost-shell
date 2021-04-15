#ifndef GHOST_SHELL_GHOSTSH_H
#define GHOST_SHELL_GHOSTSH_H

#include <stddef.h>
#include <stdio.h>
#include "ghostshstrings.h"

#define GS_OPT_SELFCHECK 0x01
#define GS_OPT_DEBUG 0x02

#define gs_check_opt(options, opt) (options->flags & opt)
#define gs_debug_log(options, ...) \
    if (gs_check_opt(options, GS_OPT_DEBUG)) \
    { printf(__VA_ARGS__); printf("\n"); };

typedef struct {
    unsigned int flags;
    const char *prompt;
    char *selfcheck_export_path;
    char *envp[];
} gs_options_t;

gs_options_t* gs_options_new();
void gs_options_free(gs_options_t*);
void gs_parse_options(gs_options_t* opt, int argc, char *argv[]);
void gs_print_usage_exit(void);
void gs_run_selfcheck(gs_options_t* opt);

int gs_run_interactively(gs_options_t* opt);

#define gs_CMD_INFO_BLANK 1

typedef struct gs_command_line {
    int argc;
    char **argv;
    struct gs_command_line* pipe_to;
} gs_command_line_t;

gs_command_line_t* gs_command_line_new(void);
void gs_command_line_free(gs_command_line_t* cmd);
void gs_command_line_clear(gs_command_line_t* cmd);
void gs_command_line_set_argv(gs_command_line_t* cmd, int argc, gs_charp_list_t* argv);

#endif //GHOST_SHELL_GHOSTSH_H
