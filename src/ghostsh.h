#ifndef GHOST_SHELL_GHOSTSH_H
#define GHOST_SHELL_GHOSTSH_H

#include <stddef.h>
#include <stdio.h>
#include "ghostshstrings.h"

#define GSTSH_OPT_SELFCHECK 0x01
#define GSTSH_OPT_DEBUG 0x02

#define GSTSH_ERROR_INVALID_OPTION 1

#define gstsh_check_opt(options, opt) (options->flags & opt)
#define gstsh_debug_log(options, ...) \
    if (gstsh_check_opt(options, GSTSH_OPT_DEBUG)) \
    { printf(__VA_ARGS__); printf("\n"); };

#define gstsh_panic(opt, macrocode, ...) \
    do { \
        gstsh_debug_log(opt, '*', "Error (%s) in %s, line %d:\n", #macrocode, __FILE__, __LINE__);  \
        printf(__VA_ARGS__); \
        printf("\n"); \
       exit(macrocode); \
    } while(0);

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

#define GSTSH_CMD_INFO_SUCCESS 0x01
#define GSTSH_CMD_INFO_BLANK 0x02

typedef struct {
    int info_flags;
    int argc;
    char **argv;
} gstsh_command_line_t;

#define gstsh_command_line_check_info(cmd, info) (cmd->info_flags & info)
#define gstsh_command_line_set_info(cmd, info) (cmd->info_flags |= info)

gstsh_command_line_t* gstsh_command_line_new(void);
void gstsh_command_line_free(gstsh_command_line_t* cmd);
void gstsh_command_line_clear(gstsh_command_line_t* cmd);
void gstsh_command_line_set_argv(gstsh_command_line_t* cmd, int argc, gstsh_charp_list_t* argv);

#endif //GHOST_SHELL_GHOSTSH_H
