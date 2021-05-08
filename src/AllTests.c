
/* This is auto-generated code. Edit at your own peril. */
#include <stdio.h>
#include <stdlib.h>

#include "CuTest.h"

extern void TestAcceptanceParseOptions(CuTest*);
extern void TestAcceptanceParseShouldIdentifyProgram(CuTest*);
extern void TestAcceptancePipesShouldCreateCmdLineTree(CuTest*);
extern void TestAcceptanceShRunCorrectly(CuTest*);
extern void TestPipeSupport(CuTest*);

int RunAllTests(char* exportJunitFormat)
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, TestAcceptanceParseOptions);
    SUITE_ADD_TEST(suite, TestAcceptanceParseShouldIdentifyProgram);
    SUITE_ADD_TEST(suite, TestAcceptancePipesShouldCreateCmdLineTree);
    SUITE_ADD_TEST(suite, TestAcceptanceShRunCorrectly);
    SUITE_ADD_TEST(suite, TestPipeSupport);

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    if (exportJunitFormat != NULL)
    {
      CuString *export = CuStringNew();
      CuSuiteExportJunitXml(suite, export);
      FILE *fp = fopen(exportJunitFormat, "w+");
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

