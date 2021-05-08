#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ghostsh.h"

gs_context_t* gs_context_new(void)
{
    gs_context_t* ctx = malloc(sizeof(gs_context_t));
    gs_context_reset(ctx);

    return ctx;
}

void gs_context_register_running_pid(gs_context_t* ctx, pid_t pid)
{
    if (ctx->n_running_pids == GS_MAX_RUNNING_PIDS)
    {
        gs_panic(GS_ERR_MAX_PIDS, "Max running process reached.");
    }

    ctx->running[ctx->n_running_pids] = pid;
    ctx->n_running_pids++;
}

void gs_context_register_open_pipe(gs_context_t* ctx, int pipe[2])
{
    if (ctx->n_open_pipes == GS_MAX_RUNNING_PIDS)
    {
        gs_panic(GS_ERR_MAX_PIPES, "Max pipes open reached.");
    }

    ctx->open_pipes[ctx->n_open_pipes][0] = pipe[0];
    ctx->open_pipes[ctx->n_open_pipes][1] = pipe[1];
    ctx->n_open_pipes++;
}

void gs_context_close_pipes(gs_context_t* ctx)
{
    for (int i = ctx->n_open_pipes - 1; i >= 0; --i)
    {
        close(ctx->open_pipes[i][0]);
        close(ctx->open_pipes[i][1]);
        ctx->open_pipes[i][0] = -1;
        ctx->open_pipes[i][1] = -1;
    }

    ctx->n_open_pipes = 0;
}

void gs_context_wait_for_child_pids(gs_context_t* ctx)
{
    for (int i = ctx->n_running_pids - 1; i >=0; --i)
    {
        waitpid(ctx->running[i], NULL, 0);
    }

    ctx->n_running_pids = 0;
}

void gs_context_reset(gs_context_t* ctx)
{
    ctx->n_running_pids = 0;
    memset(&ctx->running, 0, sizeof(pid_t) * GS_MAX_RUNNING_PIDS);

    ctx->n_open_pipes = 0;
    memset(&ctx->open_pipes, -1, sizeof(int[2]) * GS_MAX_RUNNING_PIDS);

    ctx->options = NULL;
}

void gs_context_free(gs_context_t* ctx)
{
    free(ctx);
}