//Copyright 2025 howdy213
//the MIT License
//Author: howdy213
#include "astronomy.h"
#include "AstronomyTest.h"
#include "CalendarMath.h"
#include <stdio.h>

#define DISPLAY_AS_CSTB 0

void calc_chn_cal(int y) {
    constexpr int WINTER_SOLSTICE_INDEX = 23 - 5;
    const astronomy::REAL tz = y >= 1929 ? TIMEZONE_BEIJING : TIMEZONE_BEIJING_LOCAL;

    struct MyDayTime {
        Time t;
        int ofst;

        void set(astronomy::REAL jd) {
            astronomy::daytime_from_julian_day(jd - astronomy::calc_delta_t(jd), &t);
            ofst = days_offset(t);
        }
    };


    // 由于农历的置闰是以冬至为锚点的，11、12月是否闰取决于上一个周期，而1~10月是否闰取决于下一个周期
    // 这里为了显示，把节气也显示出来，所以需要24*2，多出来的3是上一年的小雪、大雪、冬至
    // 朔日需要本来只需要计算26个，又因为如果冬至离朔日很近的时候，可能迭代到上一个月的，加之腊月需要显示大小，故有28
    MyDayTime solar_terms[51]{}, new_moons[28]{};

    // 上年冬至、以及上年冬至之前的朔
    astronomy::REAL jd_st = calc_solar_term(y - 1, WINTER_SOLSTICE_INDEX);
    astronomy::REAL jd_nm = calc_new_moon_nearby(estimate_new_moon_backward(jd_st));
    int st_idx = 2, nm_idx = 1;


    // 下标0和1是小雪、大雪，这两个有可能跟冬至在同一个月（概率较小）
    // 下标0预留冬至之前的朔
    solar_terms[2].set(jd_st + tz);
    new_moons[1].set(jd_nm + tz);

    // 如果朔比冬至大，则说明迭代到下一个月的朔了，需要检查更早一个朔
    if (new_moons[1].ofst > solar_terms[2].ofst) {
        astronomy::REAL jd_tmp = calc_new_moon_nearby(jd_nm - 29.53);
        new_moons[0].set(jd_tmp + tz);
        if (new_moons[0].ofst < solar_terms[2].ofst) {
            nm_idx = 0;
        }
    }
    else {
        astronomy::REAL jd_tmp = calc_new_moon_nearby(jd_nm + 29.53);
        new_moons[2].set(jd_tmp + tz);
        if (new_moons[2].ofst == solar_terms[2].ofst) {
            new_moons[1] = new_moons[2];
            jd_nm = jd_tmp;
        }
    }

    // 上年小雪、大雪
    for (int i = 0; i < 2; ++i) {
        jd_st = calc_solar_term(y - 1, (WINTER_SOLSTICE_INDEX + 22 + i) % 24);
        solar_terms[i].set(jd_st + tz);
    }

    // 上年冬至~今年冬至
    for (int i = 0; i < 24; ++i) {
        jd_st = calc_solar_term(y, i >= 5 ? i - 5 : i + 19);
        solar_terms[i + 3].set(jd_st + tz);
    }

    // 今年冬至~次年冬至
    for (int i = 0; i < 24; ++i) {
        jd_st = calc_solar_term(y + 1, i >= 5 ? i - 5 : i + 19);
        solar_terms[i + 27].set(jd_st + tz);
    }

    // 朔
    for (int i = 2; i < 28; ++i) {
        jd_nm = calc_new_moon_nearby(jd_nm + 29.53);
        new_moons[i].set(jd_nm + tz);
    }

    printf("%d\n", y);

#if 0
    printf("solar terms:\n");
    for (const auto &st : solar_terms) {
        print_daytime(st.t);
        printf("\n");
    }

    printf("new moons:\n");
    for (int i = nm_idx; i < 28; ++i) {
        print_daytime(new_moons[i].t);
        printf("\n");
    }
#endif

    int leap = 0;

    // 闰月在上年冬至~今年冬至区间
    // solar_terms[26]为今年冬至
    if (solar_terms[26].ofst >= new_moons[nm_idx + 13].ofst) {
        int ms_idx = 2, o;
        for (int i = 0; i < 13; ++i) {
            o = solar_terms[ms_idx].ofst;
            if (new_moons[nm_idx + i + 1].ofst <= o) {
                leap = nm_idx + i;
                break;
            }
            ms_idx += 2;
        }
    }
    // 闰月在今年冬至~下年冬至区间
    // solar_terms[50]为下年冬至
    else if (solar_terms[50].ofst >= new_moons[nm_idx + 25].ofst) {
        int ms_idx = 26, o;
        for (int i = 0; i < 13; ++i) {
            o = solar_terms[ms_idx].ofst;
            if (new_moons[nm_idx + i + 13].ofst <= o) {
                leap = nm_idx + i + 12;
                break;
            }
            ms_idx += 2;
        }
    }

    st_idx = 0;
    nm_idx = 2;
    while (nm_idx < 16) {
        const auto &mn0 = new_moons[nm_idx];
        const auto &mn1 = new_moons[nm_idx + 1];
        bool major = (mn1.ofst - mn0.ofst) == 30;

        int readable;
        if (leap == 0 || nm_idx < leap) {
            // 无闰、或本月在闰月之前
            readable = (nm_idx + 9) % 12 + 1;
        }
        else {
            // 有闰且本月在闰月或者之后
            readable = (nm_idx + 8) % 12 + 1;
        }

#if 1
        // 按岁显示 +1
        if (nm_idx >  (leap == 0 || nm_idx <= leap ? 13 : 14)) {
            break;
        }
#endif

#if 0
        // 按年显示
        if (leap == 0 ? (nm_idx < 3 || nm_idx > 14) : (nm_idx < (leap >= 4 ? 3 : 4) || nm_idx > (leap <= 15 ? 15 : 14))) {
            ++nm_idx;
            continue;
        }
#endif


#if DISPLAY_AS_CSTB
        // 汉字方式显示月份 天支日期
        printf("%s%s%s ", leap == 0 || nm_idx != leap ? "　" : "閏", month_names[readable - 1], major ? "大" : "小");
        print_daytime_cstb(mn0.t);
#else
#if 1
        // 数字方式显示月份
        printf("%c", leap == 0 || nm_idx != leap ? ' ' : '+');
        printf("%.2d ", readable);
        printf("%c (", major ? '+' : '-');
        print_daytime(mn0.t);
        printf(")");
#endif

#if 0
        // 汉字方式显示月份
        printf("%s%s%s (", leap == 0 || nm_idx != leap ? "　" : "閏", month_names[readable - 1], major ? "大" : "小");
        print_daytime(mn0.t);
        printf(")");
#endif
#endif

        ++nm_idx;

        // 显示节气
        while (1) {
            const auto &st = solar_terms[st_idx];

            // 节气超过本月了
            if (st.ofst >= mn1.ofst) {
                break;
            }

            // 节气落在本月内
            if (st.ofst >= mn0.ofst) {
#if DISPLAY_AS_CSTB
                printf(" %s", day_names[st.ofst - mn0.ofst]);
                print_daytime_cstb(st.t);
                printf("%s", solar_terms_names[(st_idx + 21) % 24]);
#else
                printf(" %s (", solar_terms_names[(st_idx + 21) % 24]);
                print_daytime(st.t);
                printf(")");
#endif
            }

            ++st_idx;
        }

        printf("\n");
    }

}
