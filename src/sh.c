#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ghostsh.h"
#include "ghostshparser.h"
#include "CuTest.h"

int _gstsh_run_for_fds(gstsh_options_t* opt, FILE* in, FILE* out)
{
    char line[4096];

    gstsh_command_line_t* cmd = gstsh_command_line_new();

    while (1)
    {
        fprintf(out,"%s ", opt->prompt);
        fflush(out);
        if (fgets(line, 4095, in) == NULL)
        {
            break;
        }
        line[strlen(line) - 1] = '\0';

        gstsh_parse_line(line, cmd);

        if (cmd->argc == 0)
        {
            gstsh_command_line_clear(cmd);
            continue;
        }

        pid_t pid = fork();
        if (pid == 0)
        {
            dup2(fileno(in), 0);
            dup2(fileno(out), 1);
            dup2(fileno(out), 2);
            if (execvp(cmd->argv[0], cmd->argv) == -1)
            {
                fprintf(out, "%s: %s[%d].\n", cmd->argv[0], strerror(errno), errno);
                return errno;
            }
        } else if (pid > 0)
        {
            int status;
            pid_t dead_child = waitpid(pid, &status, 0);
        } else
        {
            fprintf(out, "Could not start process: %d\n", pid);
        }

        gstsh_command_line_clear(cmd);
    }

    gstsh_command_line_free(cmd);

    return 0;
}


int gstsh_run_interactively(gstsh_options_t* opt)
{
    return _gstsh_run_for_fds(opt, stdin, stdout);
}


void TestAcceptanceShRunCorrectly(CuTest* tc)
{
    gstsh_options_t* opt = gstsh_options_new();
    opt->prompt = "|";

    FILE* mockin = tmpfile();
    FILE* mockout = tmpfile();

    fputs("\n", mockin);
    rewind(mockin);
    _gstsh_run_for_fds(opt, mockin, mockout);
    CuAssertFileContents_Msg(tc, "Check empty line produces one prompt and then exits", "| | ", mockout);

    rewind(mockin);
    fputs("echo baaa\n", mockin);
    rewind(mockin);
    rewind(mockout);
    _gstsh_run_for_fds(opt, mockin, mockout);
    CuAssertFileContents_Msg(tc, "Check is able to execute child program", "| baaa\n| ", mockout);

    gstsh_options_free(opt);

    fclose(mockin);
    fclose(mockout);
}