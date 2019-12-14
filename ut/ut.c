#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>
#include <google/cmockery.h>
#include "../src/util_time.h"

static void test_test(void **state)
{
	assert_true(true);
	assert_false(false);
}

static void test_getostime(void **state)
{
	uint32_t time = getostime();

	printf("time = %d\n", getostime());

	assert_true(time <= getostime());
}

static void test_ostime_delay(void **state)
{
	uint32_t time = getostime();
	uint32_t delay_us = 10;
	uint32_t delay_ms = OS_MS(2);
	uint32_t delay_s = OS_SEC(2);

	ostime_delay(delay_us);
	assert_true(getostime() - time >= delay_us);

	ostime_delay(delay_ms);
	assert_true(getostime() - time >= delay_ms);

	ostime_delay(delay_s);
	assert_true(getostime() - time >= delay_s);
}

static void test_print_ostime(void **state)
{
	print_ostime();
}

static void test_getostimestr(void **state)
{
	char str[128] = {'\0'};

	assert_true(getostimestr(str));

	assert_true(strlen(str));

	printf("%s\n", str);
}

static void test_getostime_format(void **state)
{
	struct tm *tm = NULL;

	assert_true(getostime_format(&tm));
	assert_true(tm);
}

static void test_get_serail_time(void **state)
{
	struct seria_time time;

	memset(&time, 0, sizeof(time));

	assert_true(get_serail_time(&time));
}

void ut_run(void)
{
	UnitTest test[] = {
		unit_test_setup_teardown(test_test, NULL, NULL),
		unit_test_setup_teardown(test_getostime, NULL, NULL),
		unit_test_setup_teardown(test_ostime_delay, NULL, NULL),
		unit_test_setup_teardown(test_print_ostime, NULL, NULL),
		unit_test_setup_teardown(test_getostimestr, NULL, NULL),
		unit_test_setup_teardown(test_getostime_format, NULL, NULL),
		unit_test_setup_teardown(test_get_serail_time, NULL, NULL),
	};

	run_tests(test);
}