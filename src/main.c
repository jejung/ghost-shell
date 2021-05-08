#include "ghostsh.h"

int main(int argc, char *argv[])
{
    gs_options_t* options = gs_options_new();
    gs_parse_options(options, argc, argv);
    gs_debug_log(options, "Verbose output enabled")

    if (gs_check_opt(options, GS_OPT_SELFCHECK))
    {
        gs_run_selfcheck(options);
    }

    int result_code = gs_run_interactively(options);
    gs_options_free(options);

    return result_code;
}
