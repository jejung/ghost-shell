#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "ghostsh.h"
#include "ghostshparser.h"
#include "ghostshstrings.h"
#include "CuTest.h"

void gs_parse_line(char* buf, gs_command_line_t* cmd)
{
    char* end_pipes;
    char* pipes = strtok_r(buf, "|", &end_pipes);

    while (pipes != NULL) {
        char* end_token;
        char* token = strtok_r(pipes, " \t", &end_token);

        gs_charp_list_t* argv = gs_charp_list_new();
        gs_charp_list_t* node = argv;

        int argc = 0;
        while (token != NULL) {
            node->data = token;
            node->next = gs_charp_list_new();
            node = node->next;
            argc++;
            token = strtok_r(NULL, " \t", &end_token);
        }

        gs_command_line_set_argv(cmd, argc, argv);
        gs_charp_list_free(argv);

        pipes = strtok_r(NULL, "|", &end_pipes);
        if (pipes == NULL)
        {
            break;
        }

        cmd->pipe_to = gs_command_line_new();
        cmd = cmd->pipe_to;
    }
}


void TestAcceptanceParseShouldIdentifyProgram(CuTest* tc)
{
    char* buf = malloc(sizeof(char) * 500);
    gs_command_line_t* cmd = gs_command_line_new();

    strcpy(buf, "");
    gs_parse_line(buf, cmd);
    CuAssert(tc, "Check blank string is set as blank", cmd->argc == 0);
    gs_command_line_clear(cmd);

    strcpy(buf, " \t");
    gs_parse_line(buf, cmd);
    CuAssert(tc, "Check whitespace string is set as blank", cmd->argc == 0);
    gs_command_line_clear(cmd);

    strcpy(buf, "ls");
    gs_parse_line(buf, cmd);
    CuAssert(tc, "Check single-program string is set as not blank", cmd->argc > 0);
    CuAssertStrEquals_Msg(tc, "Check single-program program is recognized", "ls", cmd->argv[0]);
    gs_command_line_clear(cmd);

    strcpy(buf, " ls \t");
    gs_parse_line(buf, cmd);
    CuAssertStrEquals_Msg(tc, "Check program is trimmed", "ls", cmd->argv[0]);
    gs_command_line_clear(cmd);

    strcpy(buf, "program first-arg second-arg");
    gs_parse_line(buf, cmd);
    CuAssertIntEquals_Msg(tc, "Check argc is correctly set", 3, cmd->argc);
    CuAssertStrEquals_Msg(tc, "Check program name is correctly set", "program", cmd->argv[0]);
    CuAssertStrEquals_Msg(tc, "Check first arg is correctly set", "first-arg", cmd->argv[1]);
    CuAssertStrEquals_Msg(tc, "Check second arg is correctly set", "second-arg", cmd->argv[2]);
    CuAssertPtrEquals_Msg(tc, "Check an additional entry is set as null on argv to close it", NULL, cmd->argv[3]);
    gs_command_line_clear(cmd);

    gs_command_line_free(cmd);
    free(buf);
}

void TestAcceptancePipesShouldCreateCmdLineTree(CuTest* tc)
{
    char *buf = malloc(sizeof(char) * 500);
    gs_command_line_t *cmd = gs_command_line_new();

    strcpy(buf, "program1 | program2");
    gs_parse_line(buf, cmd);
    CuAssertPtrNotNullMsg(tc, "Check leaf exists", cmd->pipe_to);
    CuAssertStrEquals_Msg(tc, "Check program name is correctly set to root", "program1", cmd->argv[0]);
    CuAssertStrEquals_Msg(tc, "Check program name is correctly set to leaf", "program2", cmd->pipe_to->argv[0]);
    gs_command_line_clear(cmd);

    strcpy(buf, "program1 arg1| program2 arg2 --opt2");
    gs_parse_line(buf, cmd);
    CuAssertPtrNotNullMsg(tc, "Check leaf exists", cmd->pipe_to);
    CuAssertIntEquals_Msg(tc, "Check argc is correctly set to root", 2, cmd->argc);
    CuAssertStrEquals_Msg(tc, "Check program name is correctly set to root", "program1", cmd->argv[0]);
    CuAssertStrEquals_Msg(tc, "Check arguments are correctly set to root", "arg1", cmd->argv[1]);
    CuAssertIntEquals_Msg(tc, "Check argc is correctly set to leaf", 3, cmd->pipe_to->argc);
    CuAssertStrEquals_Msg(tc, "Check program name is correctly set to leaf", "program2", cmd->pipe_to->argv[0]);
    CuAssertStrEquals_Msg(tc, "Check arguments are correctly set to leaf", "arg2", cmd->pipe_to->argv[1]);
    CuAssertStrEquals_Msg(tc, "Check arguments are correctly set to leaf", "--opt2", cmd->pipe_to->argv[2]);
    gs_command_line_clear(cmd);

    gs_command_line_free(cmd);
    free(buf);
}