#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ghostsh.h"
#include "ghostshparser.h"
#include "CuTest.h"


void _prepare_io_chain(gs_context_t* ctx, gs_command_line_t* cmd, int infno, int outfno)
{
    cmd->in = infno;
    cmd->out = outfno;

    if (cmd->pipe_to != NULL)
    {
        int pipefds[2];
        pipe(pipefds);
        gs_context_register_open_pipe(ctx, pipefds);
        cmd->out = pipefds[1];
        _prepare_io_chain(ctx, cmd->pipe_to, pipefds[0], outfno);
    }
}

pid_t _run_in_separate_process(gs_context_t* ctx, gs_command_line_t* cmd)
{
    if (cmd == NULL)
    {
        return 0;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        printf("Could not start process for %s: %d\n", cmd->argv[0], pid);
        return pid;
    }

    if (pid == 0)
    {
        dup2(cmd->in, STDIN_FILENO);
        dup2(cmd->out, STDOUT_FILENO);

        gs_context_close_pipes(ctx);

        if (execvp(cmd->argv[0], cmd->argv) == -1)
        {
            printf("%s: %s[%d].\n", cmd->argv[0], strerror(errno), errno);
            return -errno;
        }
    }

    return pid;
}

void _run_cmd_line(gs_context_t* ctx, gs_command_line_t* cmd, int infno, int outfno)
{
    _prepare_io_chain(ctx, cmd, infno, outfno);

    gs_command_line_t* current = cmd;

    while (current != NULL)
    {
        pid_t pid = _run_in_separate_process(ctx, current);

        if (pid == 0)
        {
            gs_panic(GS_ERR_UNEXPECTED, "Something really bad happened, child process broke confinement.");
        }

        if (pid > 0)
        {
            gs_debug_log(
                ctx->options,
                 "Running pid %d[program: %s, in: %d, out: %d]",
                 pid,
                 current->argv[0],
                 current->in,
                 current->out
             );
            gs_context_register_running_pid(ctx, pid);
        }

        current = current->pipe_to;
    }

    gs_context_close_pipes(ctx);
    gs_context_wait_for_child_pids(ctx);
}

int _gs_run_for_fds(gs_options_t* opt, FILE* in, FILE* out)
{
    char line[4096];

    gs_command_line_t* cmd = gs_command_line_new();
    gs_context_t* ctx = gs_context_new();
    ctx->options = opt;
    gs_parse_result_t result;

    while (1)
    {
        fprintf(out,"%s ", opt->prompt);
        fflush(out);
        if (fgets(line, sizeof(line) - 1, in) == NULL)
        {
            break;
        }
        line[strlen(line) - 1] = '\0';

        result = gs_parse_line(line, cmd);

        if (result.status != GS_PARSE_SUCCESS)
        {
            if (result.msg != NULL)
            {
                fprintf(out, "%s\n", result.msg);
            }
            gs_command_line_clear(cmd);
            continue;
        }

        _run_cmd_line(ctx, cmd, fileno(in), fileno(out));
        gs_command_line_clear(cmd);
    }

    gs_command_line_free(cmd);
    gs_context_free(ctx);

    return 0;
}


int gs_run_interactively(gs_options_t* opt)
{
    return _gs_run_for_fds(opt, stdin, stdout);
}

#define PREPARE_TEST(pmpt) \
    gs_options_t* opt = gs_options_new(); \
    opt->prompt = (pmpt); \
    FILE* mockin = tmpfile(); \
    FILE* mockout = tmpfile();

#define TEST_SCENARIO(input) \
    ftruncate(fileno(mockin), 0); \
    ftruncate(fileno(mockout), 0);  \
    rewind(mockin); \
    rewind(mockout); \
    fputs((input), mockin); \
    fputs("\n", mockin); \
    rewind(mockin); \
    _gs_run_for_fds(opt, mockin, mockout);

#define END_TEST() \
    gs_options_free(opt); \
    fclose(mockin); \
    fclose(mockout);

void TestAcceptanceShRunCorrectly(CuTest* tc)
{
    PREPARE_TEST("|");

    TEST_SCENARIO("")
    CuAssertFileContents_Msg(tc, "Check empty line produces one prompt and then exits", "| | ", mockout);

    TEST_SCENARIO("echo baaa")
    CuAssertFileContents_Msg(tc, "Check is able to execute child program", "| baaa\n| ", mockout);

    END_TEST();
}

void TestPipeSupport(CuTest* tc)
{
    PREPARE_TEST(">>");

    TEST_SCENARIO("|");
    CuAssertFileContents_Msg(tc, "Check invalid pipe usage is reported", ">> Invalid syntax near: \"|\"\n>> ", mockout);

    TEST_SCENARIO("program |");
    CuAssertFileContents_Msg(tc, "Check invalid pipe usage missing program on the end", ">> Invalid syntax near: \"|\"\n>> ", mockout);

    TEST_SCENARIO("echo \"there are 5 words here\" | wc -w");
    CuAssertFilePattern_Msg(tc, "Check wc is executed with the correct arguments and inputs", ">> %s\n>> ", "5", mockout);

    END_TEST();
}

void TestEscapedInputSupport(CuTest* tc)
{
    PREPARE_TEST(">>");

    TEST_SCENARIO("echo abc\"ac\"");
    CuAssertFileContents_Msg(tc, "Check unclosed quote is identified", ">> Invalid syntax near: \"\"\"\n>> ", mockout);

    TEST_SCENARIO("echo \"");
    CuAssertFileContents_Msg(tc, "Check unclosed quote is identified", ">> Invalid syntax: quote was not closed\n>> ", mockout);

    END_TEST();
}
