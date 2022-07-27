#ifndef GHOST_SHELL_GHOSTSHPARSER_H
#define GHOST_SHELL_GHOSTSHPARSER_H

#include "ghostsh.h"

#define GS_PARSE_SUCCESS 0
#define GS_PARSE_INVALID_SYNTAX 1
#define GS_PARSE_EMPTY_LINE 2

typedef struct {
    int status;
    const char* msg;
} gs_parse_result_t;

gs_parse_result_t gs_parse_line(char* buf, gs_command_line_t* cmd);


#endif //GHOST_SHELL_GHOSTSHPARSER_H
