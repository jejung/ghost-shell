
/* This is auto-generated code. Edit at your own peril. */
#include <stdio.h>
#include <stdlib.h>

#include "ghostsh.h"
#include "CuTest.h"

extern void TestAcceptanceParseOptions(CuTest*);
extern void TestAcceptanceParseShouldIdentifyProgram(CuTest*);
extern void TestAcceptancePipesShouldCreateCmdLineTree(CuTest*);
extern void TestAcceptanceQuotesShouldWorkForGroupingParameters(CuTest*);
extern void TestAcceptanceShRunCorrectly(CuTest*);
extern void TestPipeSupport(CuTest*);
extern void TestEscapedInputSupport(CuTest*);

int RunAllTests(gs_options_t* opt)
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();
    suite->opt = opt;

    SUITE_ADD_TEST(suite, TestAcceptanceParseOptions);
    SUITE_ADD_TEST(suite, TestAcceptanceParseShouldIdentifyProgram);
    SUITE_ADD_TEST(suite, TestAcceptancePipesShouldCreateCmdLineTree);
    SUITE_ADD_TEST(suite, TestAcceptanceQuotesShouldWorkForGroupingParameters);
    SUITE_ADD_TEST(suite, TestAcceptanceShRunCorrectly);
    SUITE_ADD_TEST(suite, TestPipeSupport);
    SUITE_ADD_TEST(suite, TestEscapedInputSupport);

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    if (opt->selfcheck_export_path != NULL)
    {
      CuString *export = CuStringNew();
      CuSuiteExportJunitXml(suite, export);
      FILE *fp = fopen(opt->selfcheck_export_path, "w+");
      fwrite(export->buffer, export->length, 1, fp);
      fclose(fp);
      CuStringDelete(export);
    }

    int failures = suite->failCount;
    printf("%s\n", output->buffer);
    CuStringDelete(output);
    CuSuiteDelete(suite);

    return failures;
}

