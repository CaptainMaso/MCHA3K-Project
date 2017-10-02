#include <unity_fixture.h>
#include "iospy.h"

#include "cmd_parser.h"
#include <string.h>

TEST_GROUP(CmdParse);

TEST_SETUP(CmdParse)
{
}

TEST_TEAR_DOWN(CmdParse)
{
}

TEST(CmdParse, ParseNull)
{
    char str_actual[80];

    iospy_hook_out();
    cmd_parse(0, NULL);
    rewind(stdout);
    fgets(str_actual,sizeof(str_actual),iospy_get_fp_out());
    iospy_unhook_out();

    TEST_ASSERT_EQUAL_STRING("ERROR: Tried to parse 0 arguments\n",str_actual);
}

TEST(CmdParse, ParseUnknown)
{
	const char* words[] = {"this", "is", "a", "test"};
	int len = 4;

    char str_actual[80];

    iospy_hook_out();
    cmd_parse(len, words);
    iospy_pop_out_str(str_actual, sizeof(str_actual)/sizeof(str_actual[0]));
    // rewind(stdout);
    // fgets(str_actual,sizeof(str_actual),iospy_get_fp_out());
    iospy_unhook_out();

    char str_expected[1024];
    const char * fmt = "\tERROR: Unknown command \"%s\"\n";
    snprintf(str_expected, sizeof(str_expected), fmt, (words[0]));
    TEST_ASSERT_EQUAL_STRING(str_expected,str_actual);
}

TEST(CmdParse, ParseUnknownTwice)
{
    const char* cmd[] = {"not", "a", "known", "command", "string"};
	uint8_t count = sizeof(cmd)/sizeof(char*);
    char str_actual1[80];
    char str_actual2[80];

    iospy_hook_out();
    cmd_parse(count, cmd);
    iospy_pop_out_str(str_actual1, sizeof(str_actual1)/sizeof(str_actual1[0]));
    cmd_parse(count, cmd);
    iospy_pop_out_str(str_actual2, sizeof(str_actual2)/sizeof(str_actual2[0]));
    iospy_unhook_out();

    char str_expected[1024];
    const char * fmt = "\tERROR: Unknown command \"%s\"\n";
    snprintf(str_expected, sizeof(str_expected), fmt, cmd[0]);
    TEST_ASSERT_EQUAL_STRING(str_expected, str_actual1);
    TEST_ASSERT_EQUAL_STRING(str_expected, str_actual2);
}

TEST_GROUP_RUNNER(CmdParse)
{
    RUN_TEST_CASE(CmdParse, ParseNull);
    RUN_TEST_CASE(CmdParse, ParseUnknown);
    RUN_TEST_CASE(CmdParse, ParseUnknownTwice);
}
