# c-time

## 编译安装

>- 编译静态库:`make`，安装:`make install MAKE_INSTALL_PREFIX=./install`
>- 编译测试:`make ITEM=TEST`

## 时间说明

先看看时间的标准：

- 世界时:
世界时是最早的时间标准。在1884年，国际上将1s确定为全年内每日平均长度的1/8.64×104。以此标准形成的时间系统，称为世界是，即UT1。1972年国际上开始使用国际原子时标，从那以后，经过格林威治老天文台本初子午线的时间便被称为世界时，即UT2，或称格林威治时间（GMT），是对地球转速周期性差异进行校正后的世界时。

- 原子时:
1967年，人们利用铯原子振荡周期极为规律的特性，研制出了高精度的原子时钟，将铯原子能级跃迁辐射9192631770周所经历的时间定为1s。现在用的时间就是1971年10月定义的国际原子时，是通过世界上大约200多台原子钟进行对比后，再由国际度量衡局时间所进行数据处理，得出的统一的原子时，简称TAI。 

- 世界协调时:
世界协调时是以地球自转为基础的时间标准。由于地球自转速度并不均匀，并非每天都是精确的86400原子s，因而导致了自转时间与世界时之间存在18个月有1s的误差。为纠正这种误差，国际地球自转研究所根据地球自转的实际情况对格林威治时间进行增减闰s的调整，与国际度量衡局时间所联合向全世界发布标准时间，这就是所谓的世界协调时（UTC:CoordinatdeUniversalTime）。UTC的表示方式为：年（y）、月（m）、日（d）、时（h）、分（min）、秒（s），均用数字表示。

linux下存储时间常见的有两种存储方式，一个是从1970年到现在经过了多少秒，一个是用一个结构来分别存储年月日时分秒的。
time_t 这种类型就是用来存储从1970年到现在经过了多少秒，要想更精确一点，可以用结构struct timeval，它精确到微妙。

```c
struct timeval
{
    long tv_sec; /*秒*/
    long tv_usec; /*微秒*/
};
```

而直接存储年月日的是一个结构：
```c

struct tm
{
    int tm_sec;  /*秒，正常范围0-59， 但允许至61*/
    int tm_min;  /*分钟，0-59*/
    int tm_hour; /*小时， 0-23*/
    int tm_mday; /*日，即一个月中的第几天，1-31*/
    int tm_mon;  /*月， 从一月算起，0-11*/  1+p->tm_mon;
    int tm_year;  /*年， 从1900至今已经多少年*/  1900＋ p->tm_year;
    int tm_wday; /*星期，一周中的第几天， 从星期日算起，0-6*/
    int tm_yday; /*从今年1月1日到目前的天数，范围0-365*/
    int tm_isdst; /*日光节约时间的旗标*/
};
```

需要特别注意的是，年份是从1900年起至今多少年，而不是直接存储如2011年，月份从0开始的，0表示一月，星期也是从0开始的， 0表示星期日，1表示星期一。

下面介绍一下我们常用的时间函数：

```c
#include <time.h>
char *asctime(const struct tm* timeptr);
```

将结构中的信息转换为真实世界的时间，以字符串的形式显示

```c
char *ctime(const time_t* timep);
```

将timep转换为真是世界的时间，以字符串显示，它和asctime不同就在于传入的参数形式不一样

```c
double difftime(time_t time1, time_t time2);
```

返回两个时间相差的秒数

```c
int gettimeofday(struct timeval* tv,struct timezone* tz);
```

返回当前距离1970年的秒数和微妙数，后面的tz是时区，一般不用

```c
struct tm* gmtime(const time_t* timep);
```

将time_t表示的时间转换为没有经过时区转换的UTC时间，是一个struct tm结构指针

```c
struct tm* localtime(const time_t* timep);
```

和gmtime类似，但是它是经过时区转换的时间。

```c
time_t mktime(struct tm* timeptr);
```

将struct tm 结构的时间转换为从1970年至今的秒数

```c
time_t time(time_t* t);
```

取得从1970年1月1日至今的秒数。

>- `int stime(time_t *t);`
>- 安全版本加`_r`后缀
