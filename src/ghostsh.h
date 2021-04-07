#ifndef GHOST_SHELL_GHOSTSH_H
#define GHOST_SHELL_GHOSTSH_H

#include <stddef.h>
#include <stdio.h>
#include "ghostshstrings.h"

#define GSTSH_OPT_SELFCHECK 0x01
#define GSTSH_OPT_DEBUG 0x02

#define gstsh_check_opt(options, opt) (options->flags & opt)
#define gstsh_debug_log(options, ...) \
    if (gstsh_check_opt(options, GSTSH_OPT_DEBUG)) \
    { printf(__VA_ARGS__); printf("\n"); };

typedef struct {
    unsigned int flags;
    const char *prompt;
    char *envp[];
} gstsh_options_t;

gstsh_options_t* gstsh_options_new();
void gstsh_options_free(gstsh_options_t*);
void gstsh_parse_options(gstsh_options_t* opt, int argc, char *argv[]);
void gstsh_print_usage_exit(void);
void gstsh_run_selfcheck(void);

int gstsh_run_interactively(gstsh_options_t* opt);

#define GSTSH_CMD_INFO_BLANK 1

typedef struct gstsh_command_line {
    int argc;
    char **argv;
    struct gstsh_command_line* pipe_to;
} gstsh_command_line_t;

gstsh_command_line_t* gstsh_command_line_new(void);
void gstsh_command_line_free(gstsh_command_line_t* cmd);
void gstsh_command_line_clear(gstsh_command_line_t* cmd);
void gstsh_command_line_set_argv(gstsh_command_line_t* cmd, int argc, gstsh_charp_list_t* argv);

#endif //GHOST_SHELL_GHOSTSH_H
