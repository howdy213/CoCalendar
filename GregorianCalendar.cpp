//Copyright 2025 howdy213
//the MIT License
//Author: howdy213
#include "GregorianCalendar.h"
#include <ctime>
using namespace std;
GregorianCalendar::GregorianCalendar() :Calendar()
{
	d_ptr->info.name = "Gregorian";
	d_ptr->info.method = "default";
	d_ptr->info.isValid = true;
	d_ptr->info.calcMode = ONLY_RECALC;
	d_ptr->info.leapMode = EXTRA_DAY;
	d_ptr->info.hasYearZero = true;
	d_ptr->info.xtDaysOfMonth = { 28,31 };
	d_ptr->info.xtDaysOfYear = { 365,366 };
	d_ptr->info.xtMonthsOfYear = { 12,12 };
	d_ptr->info.ofstDate = fromParts(1, 1, 1);
}

GregorianCalendar::~GregorianCalendar()
{
	Calendar::~Calendar();
}

int GregorianCalendar::dayOfWeek(Date t)
{
	int year = t.year;
	int month = t.mon;
	int day = t.day;
	int week = 0;
	unsigned int y = 0, c = 0, m = 0, d = 0;
	if (month == 1 || month == 2)
	{
		c = (year - 1) / 100;
		y = (year - 1) % 100;
		m = month + 12;
		d = day;
	}
	else
	{
		c = year / 100;
		y = year % 100;
		m = month;
		d = day;
	}

	week = y + y / 4 + c / 4 - 2 * c + 26 * (m + 1) / 10 + d - 1; //蔡勒公式
	week = week >= 0 ? (week % 7) : (week % 7 + 7); //week为负时取模
	if (week == 0) //星期日不作为一周的第一天
	{
		week = 7;
	}

	return week;
}

int GregorianCalendar::daysInMonth(int mon_idx, int year)
{
	if (mon_idx < 1 || mon_idx > 12) return 0; // 无效的月份
	if (year == Unspecified) year = 0; // 如果年份未指定，默认为0
	switch (mon_idx) {
	case 1: case 3: case 5: case 7: case 8: case 10: case 12:
		return 31; // 大月
	case 4: case 6: case 9: case 11:
		return 30; // 小月
	case 2:
		return isLeapYear(year) ? 29 : 28; // 闰年2月29天，平年28天
	default:
		return 0; // 无效的月份
	}
}

int GregorianCalendar::daysInYear(int year)
{
	if (year == Unspecified) return 0; // 如果年份未指定，返回0
	return isLeapYear(year) ? 366 : 365; // 闰年366天，平年365天
}

int GregorianCalendar::monthsInYear(int year) 
{
	if (year == Unspecified) return 0; // 如果年份未指定，返回0
	return 12; // 公历每年12个月
}

bool GregorianCalendar::hasYearZero() 
{
	return false; // 公历没有年零
}

bool GregorianCalendar::isLeapYear(int year) 
{
	if (year == Unspecified) return false; // 如果年份未指定，返回false
	// 公历闰年规则：能被4整除但不能被100整除，或者能被400整除
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool GregorianCalendar::isDateValid(Date t)
{
	return Calendar::isDateValid(t);
}

int GregorianCalendar::offset(Date t)
{
	if (t.mon <= 2) {
		t.mon += 12;
		t.year--;
	}
	int day = t.day;
	int y = t.year;
	int m = t.mon;
	return 365 * y + y / 4 - y / 100 + y / 400 + (153 * m - 457) / 5 + day - 307;
	//return days_offset(t);
}

/// <summary>
/// returns the difference in days between two dates.
/// d2 - d1
/// </summary>
/// <param name="d1"></param>
/// <param name="d2"></param>
/// <returns></returns>
Duration GregorianCalendar::diff(Date t1, Date t2)
{
	if (!isDateValid(t1) || !isDateValid(t2)) {
		return Duration{ Invalid }; // 如果时间无效，返回0天
	}
	int days = offset(t2) - offset(t1);

	return { days };
}

Date GregorianCalendar::move(Date t, int days)
{
	return Calendar::move(t, days);
}

string GregorianCalendar::monthName(int month, int year) 
{
	if (month < 1 || month > 12) return ""; // 无效的月份
	static  string monthNames[] = {
		"January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"
	};
	return monthNames[month - 1]; // 返回对应的月份名称
}

string GregorianCalendar::weekDayName(int day) 
{
	if (day < 1 || day > 7) return ""; // 无效的星期几
	static  string weekDayNames[] = {
		 "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday","Sunday"
	};
	return weekDayNames[day - 1]; // 返回对应的星期几名称
}

std::string GregorianCalendar::format(Date t,const  std::string& fmt)
{
	return std::to_string(t.year) + "/"
		+ std::to_string(t.mon) + "/"
		+ std::to_string(t.day);
}

