//Copyright 2025 howdy213
//the MIT License
//Author: howdy213
#include "CalendarMath.h"
#include <iostream>
using namespace std;

astronomy::REAL estimate_solar_term(int year, int angle) {
	int month = (angle + 105) / 30;
	if (month > 12) month -= 12;
	if (angle % 30 == 0) {
		return astronomy::make_julian_day(year, month, month < 8 ? 20 : 22, 0, 0,0, 0.0);
	}
	else {
		return astronomy::make_julian_day(year, month, month < 8 ? 4 : 7, 0, 0,0, 0.0);
	}
};

astronomy::REAL calc_solar_term(int year, int idx) {
	constexpr  astronomy::REAL step = 0.000005;
	constexpr  astronomy::REAL step2 = step * 2;

	astronomy::REAL JD0, JD1, D, Dp;
	int angle = idx * 15;
	JD1 = estimate_solar_term(year, angle);
	do {
		JD0 = JD1;
		D = astronomy::get_sun_ecliptic_longitude(JD0);
		D = ((angle == 0) && (D > 345.0)) ? D - 360.0 : D;

		Dp = (astronomy::get_sun_ecliptic_longitude(JD0 + step) - astronomy::get_sun_ecliptic_longitude(JD0 - step)) / step2;
		JD1 = JD0 - (D - angle) / Dp;
	} while ((fabs(JD1 - JD0) > 1e-8));

	return JD0;
}

astronomy::REAL clamp_degrees(astronomy::REAL d) {
	while (d < 0) d += 360;
	while (d > 360) d -= 360;
	return d;
}

astronomy::REAL ecliptic_longitude_diff(astronomy::REAL jd) {
	return clamp_degrees(astronomy::get_moon_ecliptic_longitude(jd) - astronomy::get_sun_ecliptic_longitude(jd));
};

astronomy::REAL estimate_new_moon_forward(astronomy::REAL jd) {
	astronomy::REAL D0, D1;
	D0 = ecliptic_longitude_diff(jd);
	for (int i = 1; i < 30; ++i) {
		jd += 1;
		D1 = ecliptic_longitude_diff(jd);
		if (D1 < D0) {
			jd -= 1;
			break;
		}
		D0 = D1;
	}
	return jd;
}

astronomy::REAL estimate_new_moon_backward(astronomy::REAL jd) {
	constexpr astronomy::REAL ONE_DAY = 360.0 / 29.53;
	constexpr astronomy::REAL ONE_DAY_RVS = 1 / ONE_DAY;

	astronomy::REAL D0, D1;
	D0 = ecliptic_longitude_diff(jd);

	if (D0 > ONE_DAY) {
		jd -= D0 * ONE_DAY_RVS;
		D1 = ecliptic_longitude_diff(jd);
		if (D1 > D0) {
			do {
				jd += 1;
				D1 = ecliptic_longitude_diff(jd);
			} while (D1 > D0);
			return jd - 1;
		}
		else if (D1 < D0) {
			D0 = D1;
			do {
				jd -= 1;
				D1 = ecliptic_longitude_diff(jd);
			} while (D1 < D0);
			return jd;
		}

		return jd;
	}

	for (int i = 1; i < 30; ++i) {
		jd -= 1;
		D1 = ecliptic_longitude_diff(jd);
		if (D1 > D0) {
			break;
		}
		D0 = D1;
	}

	return jd;
}

astronomy::REAL calc_new_moon_nearby(astronomy::REAL jd) {
	constexpr  astronomy::REAL step = 0.000005;
	constexpr  astronomy::REAL step2 = step * 2;

	astronomy::REAL JD0, JD1, D, Dp;
	JD1 = jd;
	do {
		JD0 = JD1;
		D = ecliptic_longitude_diff(JD0);
		D = (D > 345.0) ? D - 360.0 : D;

		Dp = (ecliptic_longitude_diff(JD0 + step) - ecliptic_longitude_diff(JD0 - step)) / step2;
		JD1 = JD0 - D / Dp;
	} while ((fabs(JD1 - JD0) > 1e-8));

	return JD0;
}

astronomy::REAL calc_new_moon_nearby(int year, int month, int day) {
	return calc_new_moon_nearby(astronomy::make_julian_day(year, month, day, 0, 0, 0,0.0));
}
void print_daytime(const Time& t) {
#if 1
	if ((t.hour != 0 || t.min > 30) && (t.hour != 23 || t.min < 30)) {
		printf("%.2d-%.2d %.2d:%.2d:%.2d.%3.f  ", t.mon, t.day, t.hour, t.min, t.sec,t.ms);
	}
	else {
		printf("%.2d-%.2d %.2d:%.2d:%.2d.%3.f *", t.mon, t.day, t.hour, t.min, t.sec,t.ms);
	}
#else
	printf("%.2d-%.2d", t.mon, t.day);
#endif
}

void print_daytime_cstb(const Time& t) {
	int y = t.year, m = t.mon, d = t.day;
	if (m == 1 || m == 2) {
		m += 12;
		--y;
	}
	int n = (y * 5 + (y >> 2) - y / 100 + y / 400 + ((m - 1) & 1) * 30 + (((m - 2) * 19) >> 5) + d + 8) % 60;
	printf("%s%s", celestial_stems[n % 10], terrestrial_branches[n % 12]);
}

int days_offset(const Time& t) {
	int y = t.year, m = t.mon, d = t.day;
	if (m < 3) {
		m += 12;
		--y;
	}
	return y * 365 + (y >> 2) - y / 100 + y / 400 + (m - 3) * 30 + (((m - 2) * 19) >> 5) + d - 307;
}//与Gregorian().offset()等价！

// NOTE: 一种朴素的想法，直接计算0点与24点，如果这两个时刻的值会跳转，说明节气、朔在该日
// 然而，julian_day 是有偏差的，无法反算
void calc_solar_term_for_year(int y) {
	const astronomy::REAL tz = y >= 1929 ? TIMEZONE_BEIJING : TIMEZONE_BEIJING_LOCAL;
	Time t;

	printf("// %.2d :", y % 100);
	for (int i = 0; i < 24; ++i) {
		astronomy::REAL jd = calc_solar_term(y, i >= 5 ? i - 5 : i + 19) + tz;
		astronomy::daytime_from_julian_day(jd - astronomy::calc_delta_t(jd), &t);
		printf(" %d", t.day);
	}
	printf("\n");
}

void calc_solar_term_for_year_full(int y) {
	printf("// %.2d :\n", y % 100);
	const astronomy::REAL tz = y >= 1929 ? TIMEZONE_BEIJING : TIMEZONE_BEIJING_LOCAL;
	Time t;

	for (int i = 0; i < 24; ++i) {
		astronomy::REAL jd = calc_solar_term(y, i >= 5 ? i - 5 : i + 19) + tz;
		astronomy::daytime_from_julian_day(jd - astronomy::calc_delta_t(jd), &t);

		printf("%s : ", solar_terms_names[i]);
		print_daytime(t);
		printf("\n");
	}
	printf("\n\n");
}

void calc_new_moon_for_year_full(int y) {
	const astronomy::REAL tz = y >= 1929 ? TIMEZONE_BEIJING : TIMEZONE_BEIJING_LOCAL;
	Time t;

	astronomy::REAL jd = astronomy::make_julian_day(y, 1, 1, 0, 0, 0,0.0) + tz;
	jd = calc_new_moon_nearby(estimate_new_moon_forward(jd));

	astronomy::daytime_from_julian_day(jd + tz - astronomy::calc_delta_t(jd + tz), &t);
	printf("%.2d-%.2d %.2d:%.2d:%.2d.%3.f\n", t.mon, t.day, t.hour, t.min, t.sec,t.ms);

	unsigned bit = t.day << 12;
	int offset = days_offset(t);

	for (int i = 0; i < 13; ++i) {
		jd = calc_new_moon_nearby(jd + 29.53);

		astronomy::daytime_from_julian_day(jd + tz - astronomy::calc_delta_t(jd + tz), &t);
		printf("%.2d-%.2d %.2d:%.2d:%.2d.%3.f\n", t.mon, t.day, t.hour, t.min, t.sec,t.ms);

		if (t.year == y) {
			int o = days_offset(t);
			if (o - offset == 30) {
				bit |= 1 << i;
			}
			offset = o;
		}
	}

	printf("0x%05x\n", bit);
}
