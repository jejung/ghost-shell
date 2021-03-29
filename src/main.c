#include "ghostsh.h"

int main(int argc, char *argv[])
{
    gstsh_options_t* options = gstsh_options_new();
    gstsh_parse_options(options, argc, argv);
    gstsh_debug_log(options, "Verbose output enabled")

    if (gstsh_check_opt(options, GSTSH_OPT_SELFCHECK))
    {
        gstsh_run_selfcheck();
    }

    int result_code = gstsh_run_interactively(options);

    gstsh_options_free(options);

    return result_code;
}