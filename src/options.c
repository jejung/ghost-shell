#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "ghostsh.h"
#include "CuTest.h"

gstsh_options_t* gstsh_options_new(void)
{
    gstsh_options_t* options = malloc(sizeof(gstsh_options_t));
    options->flags = 0;
    return options;
}

void gstsh_options_free(gstsh_options_t* options)
{
    free(options);
}

void gstsh_parse_options(gstsh_options_t* opt, int argc, char *argv[])
{
    static struct option long_options[] = {
        {"self-check", no_argument, 0, 0},
        {"verbose", no_argument, 0, 0},
        {0, 0, 0, 0}
    };

    static int options_flags[] = {
        GSTSH_OPT_SELFCHECK,
        GSTSH_OPT_DEBUG,
    };

    int option_index = 0;
    int c;
    while ((c = getopt_long(argc, argv, "", long_options, &option_index)) != -1)
    {
        switch (c)
        {
            case 0:
                opt->flags |= options_flags[option_index];
                break;
            case '?':
                gstsh_print_usage_exit();
                break;
        }
    }

    opt->prompt = getenv("PS1");
    if (opt->prompt == NULL)
    {
        opt->prompt = "\xF0\x9F\x91\xBB";
    }
}

void TestAcceptanceParseOptions(CuTest* tc)
{
    gstsh_options_t* opts = gstsh_options_new();
    char arg0[] = "program";
    char arg1[] = "--self-check";
    char arg2[] = "--verbose";
    char* argv[] = { &arg0[0], &arg1[0], &arg2[0], NULL };
    int argc = (int)(sizeof(argv) / sizeof(argv[0])) - 1;
    optind = 1;
    gstsh_parse_options(opts, argc, &argv[0]);

    CuAssert(tc,"self-check flag is set", opts->flags & GSTSH_OPT_SELFCHECK);
    CuAssert(tc,"verbose debug flag is set", opts->flags & GSTSH_OPT_DEBUG);
    CuAssertIntEquals_Msg(tc, "prompt is set to default", 0, strcmp("\xF0\x9F\x91\xBB", opts->prompt));

    char env[] = "PS1=WHY";
    putenv(env);
    gstsh_parse_options(opts, argc, &argv[0]);
    CuAssertIntEquals_Msg(tc, "prompt is set to PS1", 0, strcmp("WHY", opts->prompt));

    gstsh_options_free(opts);
}

void gstsh_print_usage_exit(void)
{
    printf("Usage: ghost-shell [OPTIONS]\n");
    printf("\t--self-check\tPerforms a self-check to guarantee it is safely working and then exit\n");
    exit(0);
}