#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "CuTest.h"

/*-------------------------------------------------------------------------*
 * CuStr
 *-------------------------------------------------------------------------*/

char* CuStrAlloc(int size)
{
    char* newStr = (char*) malloc(sizeof(char) * (size) );
    return newStr;
}

char* CuStrCopy(const char* old)
{
    int len = strlen(old);
    char* newStr = CuStrAlloc(len + 1);
    strcpy(newStr, old);
    return newStr;
}

/*-------------------------------------------------------------------------*
 * CuString
 *-------------------------------------------------------------------------*/

void CuStringInit(CuString* str)
{
    str->length = 0;
    str->size = STRING_MAX;
    str->buffer = (char*) malloc(sizeof(char) * str->size);
    str->buffer[0] = '\0';
}

CuString* CuStringNew(void)
{
    CuString* str = (CuString*) malloc(sizeof(CuString));
    str->length = 0;
    str->size = STRING_MAX;
    str->buffer = (char*) malloc(sizeof(char) * str->size);
    str->buffer[0] = '\0';
    return str;
}

void CuStringDelete(CuString *str)
{
        if (!str) return;
        free(str->buffer);
        free(str);
}

void CuStringResize(CuString* str, int newSize)
{
# ifdef __MACH__
    str->buffer = (char*) reallocf(str->buffer, sizeof(char) * newSize);
# else
    str->buffer = (char*) realloc(str->buffer, sizeof(char) * newSize);
# endif
    str->size = newSize;
}

void CuStringAppend(CuString* str, const char* text)
{
    int length;

    if (text == NULL) {
        text = "NULL";
    }

    length = strlen(text);
    if (str->length + length + 1 >= str->size)
        CuStringResize(str, str->length + length + 1 + STRING_INC);
    str->length += length;
    strcat(str->buffer, text);
}

void CuStringAppendChar(CuString* str, char ch)
{
    char text[2];
    text[0] = ch;
    text[1] = '\0';
    CuStringAppend(str, text);
}

void CuStringAppendFormat(CuString* str, const char* format, ...)
{
    va_list argp;
    char buf[HUGE_STRING_LEN];
    va_start(argp, format);
    vsprintf(buf, format, argp);
    va_end(argp);
    CuStringAppend(str, buf);
}

void CuStringInsert(CuString* str, const char* text, int pos)
{
    int length = strlen(text);
    if (pos > str->length)
        pos = str->length;
    if (str->length + length + 1 >= str->size)
        CuStringResize(str, str->length + length + 1 + STRING_INC);
    memmove(str->buffer + pos + length, str->buffer + pos, (str->length - pos) + 1);
    str->length += length;
    memcpy(str->buffer + pos, text, length);
}

/*-------------------------------------------------------------------------*
 * CuTest
 *-------------------------------------------------------------------------*/

void CuTestInit(CuTest* t, const char* name, TestFunction function)
{
    t->name = CuStrCopy(name);
    t->failed = 0;
    t->assertions = 0;
    t->ran = 0;
    t->message = NULL;
    t->function = function;
    t->jumpBuf = NULL;
}

CuTest* CuTestNew(const char* name, TestFunction function)
{
    CuTest* tc = CU_ALLOC(CuTest);
    CuTestInit(tc, name, function);
    return tc;
}

void CuTestDelete(CuTest *t)
{
        if (!t) return;
        free(t->name);
        free(t);
}

void CuTestRun(CuTest* tc)
{
    jmp_buf buf;
    tc->jumpBuf = &buf;
    if (setjmp(buf) == 0)
    {
        tc->ran = 1;
        (tc->function)(tc);
    }
    tc->jumpBuf = 0;
}

static void CuFailInternal(CuTest* tc, const char* file, int line, CuString* string)
{
    char buf[HUGE_STRING_LEN];

    sprintf(buf, "%s:%d: ", file, line);
    CuStringInsert(string, buf, 0);

    tc->failed = 1;
    tc->message = string->buffer;
    if (tc->jumpBuf != 0) longjmp(*(tc->jumpBuf), 0);
}

void CuFail_Line(CuTest* tc, const char* file, int line, const char* message2, const char* message)
{
    CuString string;

    CuStringInit(&string);
    if (message2 != NULL)
    {
        CuStringAppend(&string, message2);
        CuStringAppend(&string, ": ");
    }
    CuStringAppend(&string, message);
    CuFailInternal(tc, file, line, &string);
}

void CuAssert_Line(CuTest* tc, const char* file, int line, const char* message, int condition)
{
    tc->assertions++;
    if (condition) return;
    CuFail_Line(tc, file, line, NULL, message);
}

void CuAssertStrEquals_LineMsg(CuTest* tc, const char* file, int line, const char* message, 
    const char* expected, const char* actual)
{
    tc->assertions++;
    CuString string;
    if ((expected == NULL && actual == NULL) ||
        (expected != NULL && actual != NULL &&
         strcmp(expected, actual) == 0))
    {
        return;
    }

    CuStringInit(&string);
    if (message != NULL)
    {
        CuStringAppend(&string, message);
        CuStringAppend(&string, ": ");
    }
    CuStringAppend(&string, "expected <");
    CuStringAppend(&string, expected);
    CuStringAppend(&string, "> but was <");
    CuStringAppend(&string, actual);
    CuStringAppend(&string, ">");
    CuFailInternal(tc, file, line, &string);
}

void CuAssertIntEquals_LineMsg(CuTest* tc, const char* file, int line, const char* message, 
    int expected, int actual)
{
    tc->assertions++;
    char buf[STRING_MAX];
    if (expected == actual) return;
    sprintf(buf, "expected <%d> but was <%d>", expected, actual);
    CuFail_Line(tc, file, line, message, buf);
}

void CuAssertDblEquals_LineMsg(CuTest* tc, const char* file, int line, const char* message, 
    double expected, double actual, double delta)
{
    tc->assertions++;
    char buf[STRING_MAX];
    if (fabs(expected - actual) <= delta) return;
    sprintf(buf, "expected <%f> but was <%f>", expected, actual);

    CuFail_Line(tc, file, line, message, buf);
}

void CuAssertPtrEquals_LineMsg(CuTest* tc, const char* file, int line, const char* message, 
    void* expected, void* actual)
{
    tc->assertions++;
    char buf[STRING_MAX];
    if (expected == actual) return;
    sprintf(buf, "expected pointer <0x%p> but was <0x%p>", expected, actual);
    CuFail_Line(tc, file, line, message, buf);
}

void CuAssertFileContents_LineMsg(
    CuTest* tc,
    const char* file, int line, const char* message,
    const char* expected, FILE* actual
)
{
    long int pos = ftell(actual);
    fseek(actual, 0, SEEK_END);
    long int size = ftell(actual);
    char* contents = malloc(size+1);
    rewind(actual);
    int c = fread(contents, 1, size, actual);
    if (c == 0 && ferror(actual))
    {
        fprintf(stderr, "Error reading file for CuTest assertion: %d", ferror(actual));
        fclose(actual);
        exit(ferror(actual));
    }
    contents[size] = '\0';
    CuAssertStrEquals_LineMsg(tc, file, line, message, expected, contents);
    free(contents);
    fseek(actual, pos, SEEK_SET);
}

void CuAssertFilePattern_LineMsg(CuTest* tc,
                                 const char* file, int line,
                                 const char* message,
                                 const char* pattern, const char* expected, FILE* actual)
{
    const long int pos = ftell(actual);
    fseek(actual, 0, SEEK_END);
    const long int size = ftell(actual);
    if (size >= 254) {
        CuFail(tc, "File is too big for such assertion, aborting.");
        return;
    }

    char result[255];
    rewind(actual);
    fscanf(actual, pattern, &result[0]);

    CuAssertStrEquals_LineMsg(tc, file, line, message, expected, &result[0]);

    fseek(actual, pos, SEEK_SET);
}

/*-------------------------------------------------------------------------*
 * CuSuite
 *-------------------------------------------------------------------------*/

void CuSuiteInit(CuSuite* testSuite)
{
    testSuite->count = 0;
    testSuite->failCount = 0;
    testSuite->assertions = 0;
    memset(testSuite->list, 0, sizeof(testSuite->list));
}

CuSuite* CuSuiteNew(void)
{
    CuSuite* testSuite = CU_ALLOC(CuSuite);
    CuSuiteInit(testSuite);
    return testSuite;
}

void CuSuiteDelete(CuSuite *testSuite)
{
        unsigned int n;
        for (n=0; n < MAX_TEST_CASES; n++)
        {
                if (testSuite->list[n])
                {
                        CuTestDelete(testSuite->list[n]);
                }
        }
        free(testSuite);

}

void CuSuiteAdd(CuSuite* testSuite, CuTest *testCase)
{
    assert(testSuite->count < MAX_TEST_CASES);
    testSuite->list[testSuite->count] = testCase;
    testSuite->count++;
}

void CuSuiteAddSuite(CuSuite* testSuite, CuSuite* testSuite2)
{
    int i;
    for (i = 0 ; i < testSuite2->count ; ++i)
    {
        CuTest* testCase = testSuite2->list[i];
        CuSuiteAdd(testSuite, testCase);
    }
}

void CuSuiteRun(CuSuite* testSuite)
{
    gs_debug_log(testSuite->opt, "");
    int i;
    for (i = 0 ; i < testSuite->count ; ++i)
    {
        CuTest* testCase = testSuite->list[i];
        gs_debug_log(testSuite->opt, "%s", testCase->name);
        CuTestRun(testCase);
        if (testCase->failed)
        {
            gs_debug_log(testSuite->opt, "[X]");
        } else
        {
            gs_debug_log(testSuite->opt, "[✔]");
        }
        if (testCase->failed) { testSuite->failCount += 1; }
        testSuite->assertions += testCase->assertions;
    }
    gs_debug_log(testSuite->opt, "");
}

void CuSuiteSummary(CuSuite* testSuite, CuString* summary)
{
    int i;
    for (i = 0 ; i < testSuite->count ; ++i)
    {
        CuTest* testCase = testSuite->list[i];
        CuStringAppend(summary, testCase->failed ? "F" : ".");
    }
    CuStringAppend(summary, "\n\n");
}

void CuSuiteDetails(CuSuite* testSuite, CuString* details)
{
    int i;
    int failCount = 0;

    if (testSuite->failCount == 0)
    {
        int passCount = testSuite->count - testSuite->failCount;
        const char* testWord = passCount == 1 ? "test" : "tests";
        const char* assertionWord = testSuite->assertions == 1 ? "assertion" : "assertions";
        CuStringAppendFormat(details, "OK (%d %s, %d %s)\n", passCount, testWord, testSuite->assertions, assertionWord);
    }
    else
    {
        if (testSuite->failCount == 1)
            CuStringAppend(details, "There was 1 failure:\n");
        else
            CuStringAppendFormat(details, "There were %d failures:\n", testSuite->failCount);

        for (i = 0 ; i < testSuite->count ; ++i)
        {
            CuTest* testCase = testSuite->list[i];
            if (testCase->failed)
            {
                failCount++;
                CuStringAppendFormat(details, "%d) %s: %s\n",
                    failCount, testCase->name, testCase->message);
            }
        }
        CuStringAppend(details, "\n!!!FAILURES!!!\n");

        CuStringAppendFormat(details, "Runs: %d ",   testSuite->count);
        CuStringAppendFormat(details, "Passes: %d ", testSuite->count - testSuite->failCount);
        CuStringAppendFormat(details, "Fails: %d\n",  testSuite->failCount);
    }
}

void CuSuiteExportJunitXml(CuSuite* suite, CuString* to)
{
    CuStringAppendFormat(to, "<testsuite tests=\"%d\">", suite->count);
    for (int i = 0; i < suite->count; ++i)
    {
        CuTest* tc = suite->list[i];
        CuStringAppendFormat(to, "<testcase name=\"%s\">", tc->name);
        if (tc->failed)
        {
            CuStringAppendFormat(to, "<failure message=\"%s\" type=\"FailedAssertion\" />", tc->message);
        }
        CuStringAppend(to, "</testcase>");
    }
    CuStringAppend(to, "</testsuite>");
}
