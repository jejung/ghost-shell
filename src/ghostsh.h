#ifndef GHOST_SHELL_GHOSTSH_H
#define GHOST_SHELL_GHOSTSH_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "ghostshstrings.h"

#define GS_OPT_SELFCHECK 0x01
#define GS_OPT_DEBUG 0x02

#define ERR_CREATING_PROCESS 1
#define GS_ERR_MAX_PIDS 2
#define GS_ERR_MAX_PIPES 3
#define GS_ERR_UNEXPECTED 4

#define gs_check_opt(options, opt) (options->flags & opt)
#define gs_debug_log(options, ...) \
    if (gs_check_opt(options, GS_OPT_DEBUG)) \
    { printf(__VA_ARGS__); printf("\n"); };

#define gs_panic(error, ...) \
    do { printf(__VA_ARGS__); printf("\n"); exit(error);} while (0);

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
    int in;
    int out;
} gs_command_line_t;

gs_command_line_t* gs_command_line_new(void);
void gs_command_line_free(gs_command_line_t* cmd);
void gs_command_line_clear(gs_command_line_t* cmd);
void gs_command_line_set_argv(gs_command_line_t* cmd, int argc, gs_charp_list_t* argv);


#define GS_MAX_RUNNING_PIDS 50

typedef struct gs_context {
    unsigned int n_running_pids;
    pid_t running[GS_MAX_RUNNING_PIDS];

    unsigned int n_open_pipes;
    int open_pipes[GS_MAX_RUNNING_PIDS][2];

    gs_options_t* options;
} gs_context_t;

gs_context_t* gs_context_new(void);
void gs_context_register_running_pid(gs_context_t* ctx, pid_t pid);
void gs_context_register_open_pipe(gs_context_t* ctx, int pipe[2]);
void gs_context_close_pipes(gs_context_t* ctx);
void gs_context_wait_for_child_pids(gs_context_t* ctx);
void gs_context_reset(gs_context_t* ctx);
void gs_context_free(gs_context_t* ctx);

#endif //GHOST_SHELL_GHOSTSH_H
