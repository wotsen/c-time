#include <stdio.h>
#include <pthread.h>    /* -lpthread */

#include <sys/types.h>
#include <sys/time.h>

#include "util_time.h"

static bool s_time_strformat(const struct tm *tm, char *str);

static pthread_mutex_t s_time_lock = PTHREAD_MUTEX_INITIALIZER;			/* 时间读取锁 */

#if defined(LANGUAGE_ZH)
	#define TIME_FORMAT "%04d年 %02d月 %02d日 %s %02d:%02d:%02d"
	static const char *const _wday[] = {"星期天", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"};
#else
	#define TIME_FORMAT "%04d-%02d-%02d %s %02d:%02d:%02d"
	static const char *const _wday[] = {"sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"};
#endif // endof : #if defined(LANGUAGE_ZH)

/**
 * [getostime description] : 获取的是时间,微秒级别
 * @method getostime
 * @return           当前秒数(微秒)
 */
uint32_t getostime(void)
{
	uint32_t tick;
	struct timeval tv;

	pthread_mutex_lock(&s_time_lock);
	gettimeofday(&tv, NULL);
	tick = OS_SEC(tv.tv_sec) + OS_MS(tv.tv_usec / 1000);
	pthread_mutex_unlock(&s_time_lock);

	return tick;
}

/**
 * [ostime_delay description] : 指定延时
 * @method ostime_delay
 * @param  tick         [input] : 延时时间(一个tick 5微秒)
 */
void ostime_delay(uint32_t tick)
{
	struct timespec tv;

	tv.tv_sec = tick / OS_SEC(1);
	tv.tv_nsec = tick % OS_SEC(1) * TICK_NANOSEC;
	nanosleep(&tv, NULL);

	return ;
}

/**
 * [getostime_format description] : 获取一个格式化时间
 * @method getostime_format
 * @param  tm               [out] : 格式化时间
 * @return                  [out] : 格式化时间 NULL-错误 not NULL-正常
 */
bool getostime_format(struct tm **tm)
{
	time_t sec = 0;

	if (!tm) { return false; }

	if ((sec = time(NULL)) < 0)
	{
		return false;
	}

	*tm = localtime(&sec);

	return *tm ? true : false;
}

/**
 * [s_time_strformat description]	指定格式的时间格式化字符串
 * @method s_time_strformat
 * @param  tm               [input] : 当前时间
 * @param  str              [out]   : 格式化字符串
 * @return                  [out]   : 格式化字符串 NULL-出错 not NULL-成功
 */
static bool s_time_strformat(const struct tm *tm, char *str)
{
    if (!tm || !str) { return false; }

	sprintf((char *)str, TIME_FORMAT, \
			tm->tm_year + 1900, \
			tm->tm_mon + 1, \
			tm->tm_mday, \
			_wday[tm->tm_wday], \
			tm->tm_hour, \
			tm->tm_min, \
			tm->tm_sec);

    return true;
}

/**
 * [getostimestr description] 	获取当前系统时间字符串
 * @method getostimestr
 * @param  str          [out] 	: 当前时间字符串
 * @return              [out]   : NULL-出错 not NULL-成功
 */
bool getostimestr(char *str)
{
	struct tm *tm = NULL;

	if (!str) { return false; }

	if (!getostime_format(&tm)) { return false; }

    return s_time_strformat(tm, str);
}

/**
 * [get_serail_time description] 相对1900年修正过的时间
 * @method get_serail_time
 * @param  time            [description]
 * @return                 [description]
 */
bool get_serail_time(struct seria_time *time)
{
	struct tm *tm = NULL;

	if (!time) { return false; }

	if (!getostime_format(&tm)) { return false; }

	sprintf(time->tm_zone, "%s", tm->tm_zone);
	time->cur_days = tm->tm_yday;
	time->year = tm->tm_year + 1900;
	time->month = tm->tm_mon + 1;
	time->day = tm->tm_mday;
	sprintf(time->week, "%s", _wday[tm->tm_wday]);
	time->hour = tm->tm_hour;
	time->min = tm->tm_min;
	time->sec = tm->tm_sec;

	return true;
}

 /**
  * [print_ostime description] 打印当前时间
  * @method print_ostime
  */
void print_ostime(void)
{
    char str[128];
	if (!getostimestr(str))
	{
		printf("none time\n");
	}
	else
	{
		printf("%s\n", str);
	}
}

/* 打印一个月的月历 */
void print_monthly(uint8_t month)
{
    return;
}
/* 打印一年的年历 */
void print_calendar(uint16_t year)
{
    return;
}
