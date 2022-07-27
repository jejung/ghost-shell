#include <stdio.h>
#include <stdlib.h>
#include "ghostsh.h"
#include "CuTest.h"

void gs_run_selfcheck(gs_options_t *opt)
{
    printf("Self-check in progress\n");

    int failures = RunAllTests(opt);
    exit(failures);
}
