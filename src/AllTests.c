
/* This is auto-generated code. Edit at your own peril. */
#include <stdio.h>
#include <stdlib.h>

#include "CuTest.h"

extern void TestAcceptanceParseOptions(CuTest*);
extern void TestAcceptanceParseShouldIdentifyProgram(CuTest*);
extern void TestAcceptancePipesShouldCreateCmdLineTree(CuTest*);
extern void TestAcceptanceShRunCorrectly(CuTest*);

int RunAllTests(void)
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, TestAcceptanceParseOptions);
    SUITE_ADD_TEST(suite, TestAcceptanceParseShouldIdentifyProgram);
    SUITE_ADD_TEST(suite, TestAcceptancePipesShouldCreateCmdLineTree);
    SUITE_ADD_TEST(suite, TestAcceptanceShRunCorrectly);

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);

    int failures = suite->failCount;
    printf("%s\n", output->buffer);
    CuStringDelete(output);
    CuSuiteDelete(suite);

    return failures;
}

