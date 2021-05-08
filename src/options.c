#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "ghostsh.h"
#include "CuTest.h"

gs_options_t* gs_options_new(void)
{
    gs_options_t* options = malloc(sizeof(gs_options_t));
    options->flags = 0;
    options->prompt = NULL;
    options->selfcheck_export_path = NULL;
    return options;
}

void gs_options_free(gs_options_t* options)
{
    free(options);
}

void gs_parse_options(gs_options_t* opt, int argc, char *argv[])
{
    static struct option long_options[] = {
        {"self-check", no_argument, 0, 0},
        {"verbose", no_argument, 0, 0},
        {"junit-xml", required_argument, 0, 'j'},
        {0, 0, 0, 0}
    };

    static int options_flags[] = {
        GS_OPT_SELFCHECK,
        GS_OPT_DEBUG,
        0,
        0,
    };

    int option_index = 0;
    int c;
    while ((c = getopt_long(argc, argv, "", long_options, &option_index)) != -1)
    {
        switch (c)
        {
            case 'j':
                opt->selfcheck_export_path = optarg;
                break;
            case 0:
                opt->flags |= options_flags[option_index];
                break;
            case '?':
                gs_print_usage_exit();
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
    unsetenv("PS1");

    gs_options_t* opts = gs_options_new();
    char arg0[] = "program";
    char arg1[] = "--self-check";
    char arg2[] = "--verbose";
    char arg3[] = "--junit-xml";
    char arg4[] = "path/to/store/xml.xml";
    char* argv[] = { &arg0[0], &arg1[0], &arg2[0], &arg3[0], &arg4[0], NULL };
    int argc = (int)(sizeof(argv) / sizeof(argv[0])) - 1;
    optind = 1;
    gs_parse_options(opts, argc, &argv[0]);

    CuAssert(tc,"self-check flag is set", opts->flags & GS_OPT_SELFCHECK);
    CuAssert(tc,"verbose debug flag is set", opts->flags & GS_OPT_DEBUG);
    CuAssertStrEquals_Msg(tc,"junit export path is set", "path/to/store/xml.xml", opts->selfcheck_export_path);
    CuAssertIntEquals_Msg(tc, "prompt is set to default", 0, strcmp("\xF0\x9F\x91\xBB", opts->prompt));

    char env[] = "PS1=WHY";
    putenv(env);
    gs_parse_options(opts, argc, &argv[0]);
    CuAssertIntEquals_Msg(tc, "prompt is set to PS1", 0, strcmp("WHY", opts->prompt));

    gs_options_free(opts);
}

void gs_print_usage_exit(void)
{
    printf("Usage: ghost-shell [OPTIONS]\n");
    printf("\t%-30s%s\n", "--self-check", "Performs a self-check to guarantee it is safely working and then exit");
    printf("\t%-30s%s\n", "--junit-xml FILE", "Write check results to specified FILE in JUnit XML format - depends on --self-check");
    printf("\t%-30s%s\n", "--verbose", "Write debug log to stdout");
    exit(0);
}