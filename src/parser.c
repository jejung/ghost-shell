#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "ghostsh.h"
#include "ghostshparser.h"
#include "CuTest.h"

void gstsh_parse_line(char* buf, gstsh_command_line_t* cmd)
{
    char* token = strtok(buf, " \t");
    gstsh_command_line_set_info(cmd, GSTSH_CMD_INFO_SUCCESS);
    cmd->program = token;

    if (token == NULL)
    {
        gstsh_command_line_set_info(cmd, GSTSH_CMD_INFO_BLANK);
        return;
    }
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
    CuAssertStrEquals_Msg(tc, "Check single-program program is recognized", "ls", cmd->program);
    gstsh_command_line_clear(cmd);

    strcpy(buf, " ls \t");
    gstsh_parse_line(buf, cmd);
    CuAssertStrEquals_Msg(tc, "Check program is trimmed", "ls", cmd->program);
    gstsh_command_line_clear(cmd);

    gstsh_command_line_free(cmd);
}