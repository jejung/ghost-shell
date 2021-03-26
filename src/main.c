#include "ghostsh.h"
#include "CuTest.h"

int main(int argc, char** argv)
{
    gstsh_options_t* options = gstsh_options_new();
    gstsh_parse_options(options, argc, argv);
    GSTSH_DEBUG_LOG(options, "Verbose output enabled")

    if (GSTSH_CHECK_OPT(options, GSTSH_OPT_SELFCHECK))
    {
        gstsh_run_selfcheck();
    }

    return 0;
}