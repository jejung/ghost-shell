#ifndef GHOST_SHELL_GHOSTSH_H
#define GHOST_SHELL_GHOSTSH_H

#include <stddef.h>

#define GSTSH_OPT_SELFCHECK 0x01
#define GSTSH_OPT_DEBUG 0x02

#define GSTSH_ERROR_INVALID_OPTION 1

#define GSTSH_CHECK_OPT(options, opt) (options->flags & opt)
#define GSTSH_DEBUG_LOG(options, ...) \
    if (GSTSH_CHECK_OPT(options, GSTSH_OPT_DEBUG)) \
    { printf(__VA_ARGS__); printf("\n"); };

#define GSTSH_PANIC(opt, macrocode, ...) \
    do { \
        GSTSH_DEBUG_LOG(opt, '*', "Error (%s) in %s, line %d:\n", #macrocode, __FILE__, __LINE__);  \
        printf(__VA_ARGS__); \
        printf("\n"); \
       exit(macrocode); \
    } while(0);

typedef struct {
    unsigned int flags;
} gstsh_options_t;

gstsh_options_t* gstsh_options_new();
void gstsh_options_free(gstsh_options_t*);
void gstsh_parse_options(gstsh_options_t* opt, int argc, char** argv);
void gstsh_print_usage_exit(void);
void gstsh_run_selfcheck(void);

#endif //GHOST_SHELL_GHOSTSH_H
