#include <stdio.h>
#include <stdlib.h>
#include "ghostsh.h"
#include "CuTest.h"

void gstsh_run_selfcheck(void)
{
    printf("Self-check in progress\n");

    int failures = RunAllTests();
    exit(failures);
}