#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "ghostsh.h"
#include "ghostshparser.h"
#include "ghostshstrings.h"
#include "CuTest.h"

void gstsh_parse_line(char* buf, gstsh_command_line_t* cmd)
{
    char* token = strtok(buf, " \t");
    gstsh_command_line_set_info(cmd, GSTSH_CMD_INFO_SUCCESS);

    if (token == NULL)
    {
        gstsh_command_line_set_info(cmd, GSTSH_CMD_INFO_BLANK);
        return;
    }

    gstsh_charp_list_t* argv = gstsh_charp_list_new();
    gstsh_charp_list_t* node = argv;
    int argc = 0;
    do {
        node->data = token;
        node->next = gstsh_charp_list_new();
        argc++;
        node = node->next;
    } while ((token = strtok(NULL, " \t")) != NULL);

    gstsh_command_line_set_argv(cmd, argc, argv);
    gstsh_charp_list_free(argv);
}


void TestAcceptanceParseShouldIdentifyProgram(CuTest* tc)
{
    char* buf = malloc(sizeof(char) * 500);
    gstsh_command_line_t* cmd = gstsh_command_line_new();

    strcpy(buf, "");
    gstsh_parse_line(buf, cmd);
    CuAssert(tc, "Check blank string is set as success", gstsh_command_line_check_info(cmd, GSTSH_CMD_INFO_SUCCESS));
    CuAssert(tc, "Check blank string is set as blank", gstsh_command_line_check_info(cmd, GSTSH_CMD_INFO_BLANK));
    gstsh_command_line_clear(cmd);

    strcpy(buf, " \t");
    gstsh_parse_line(buf, cmd);
    CuAssert(tc, "Check whitespace string is set as success", gstsh_command_line_check_info(cmd, GSTSH_CMD_INFO_SUCCESS));
    CuAssert(tc, "Check whitespace string is set as blank", gstsh_command_line_check_info(cmd, GSTSH_CMD_INFO_BLANK));
    gstsh_command_line_clear(cmd);

    strcpy(buf, "ls");
    gstsh_parse_line(buf, cmd);
    CuAssert(tc, "Check single-program string is set as success", gstsh_command_line_check_info(cmd, GSTSH_CMD_INFO_SUCCESS));
    CuAssert(tc, "Check single-program string is set as not blank", gstsh_command_line_check_info(cmd, GSTSH_CMD_INFO_BLANK) == 0);
    CuAssertStrEquals_Msg(tc, "Check single-program program is recognized", "ls", cmd->argv[0]);
    gstsh_command_line_clear(cmd);

    strcpy(buf, " ls \t");
    gstsh_parse_line(buf, cmd);
    CuAssertStrEquals_Msg(tc, "Check program is trimmed", "ls", cmd->argv[0]);
    gstsh_command_line_clear(cmd);

    strcpy(buf, "program first-arg second-arg");
    gstsh_parse_line(buf, cmd);
    CuAssertIntEquals_Msg(tc, "Check argc is correctly set", 3, cmd->argc);
    CuAssertStrEquals_Msg(tc, "Check program name  is correctly set", "program", cmd->argv[0]);
    CuAssertStrEquals_Msg(tc, "Check first arg is correctly set", "first-arg", cmd->argv[1]);
    CuAssertStrEquals_Msg(tc, "Check second arg is correctly set", "second-arg", cmd->argv[2]);
    gstsh_command_line_clear(cmd);

    gstsh_command_line_free(cmd);
}