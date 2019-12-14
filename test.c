#include <stdio.h>
#include "src/util_time.h"

extern void ut_run(void);

int main(void)
{
	print_ostime();

	ut_run();

	return 0;
}
