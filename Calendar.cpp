//Copyright 2025 howdy213
//the MIT License
//Author: howdy213
#include "Calendar.h"
#include "GregorianCalendar.h"
Calendar::Calendar() {
	d_ptr = new CalendarPrivate();
}

Calendar::~Calendar()
{
	delete d_ptr;
	d_ptr = nullptr;
}

std::string Calendar::name()
{
	return d_ptr->info.name;
}

std::string Calendar::method()
{
	return d_ptr->info.method;
}

void Calendar::clear()
{
	d_ptr->info = CalendarInfo();
	d_ptr->info.ofstDate = fromParts(1, 1, 1);
	d_ptr->info.isValid = false;
}

bool Calendar::isValid()
{
	return d_ptr->info.isValid;
}

LeapMode Calendar::leapMode()
{
	return d_ptr->info.leapMode;
}

CalcMode Calendar::calcMode()
{
	return d_ptr->info.calcMode;
}

int Calendar::dayOfYear(Date t) {
	int days = 0;
	for (int i = 1; i < t.mon; i++) {
		days += daysInMonth(i, t.year);
	}
	days += t.day;
	return days;
}

int Calendar::monthsInYear(int year)
{
	return 12;
}

bool Calendar::hasYearZero()
{
	return d_ptr->info.hasYearZero;
}

int Calendar::leapMonth(int year)
{
	return Unspecified;
}

Date Calendar::idx2mon(int mon_idx) {
	return fromParts(Unspecified, mon_idx, Unspecified);
}

int Calendar::mon2idx(int month, bool isLeap) {
	return month;
}

bool Calendar::isDateValid(Date t) {
	if (t.year < 0) return false; // 年份不能为负
	if (t.mon < 1 || t.mon > 12) return false; // 月份必须在1到12之间
	if (t.day < 1 || t.day > daysInMonth(t.mon, t.year)) return false; // 日期必须在该月的有效范围内
	return true;
}

int Calendar::offset(Date t)
{
#if 1
	if (!isDateValid(t)) return Invalid; // 如果日期无效，返回Invalid
	int days = 0;
	days += (daysInYear(d_ptr->info.ofstDate.year) - dayOfYear(d_ptr->info.ofstDate)) + 1;
	for (int year = d_ptr->info.ofstDate.year + 1; year < t.year; ++year) {
		days += daysInYear(year);
	}
	for (int month = 1; month < t.mon; ++month) {
		days += daysInMonth(month, t.year);
	}
	days += t.day;
	return days;
#else
	return diff(d_ptr->ofstTime, t).days;
#endif
}

Date Calendar::offsetStd(Date t)
{
	Date originalDate = d_ptr->info.ofstDate;
	if (isDateValid(t))d_ptr->info.ofstDate = t;
	return originalDate;
}

Duration Calendar::diff(Date t1, Date t2)
{
#if 1
	int days = offset(t1) - offset(t2);
	return { days };
#else 
	if (t1.year == t2.year)return { dayOfYear(t2) - dayOfYear(t1) };
	bool sign = t1 < t2;
	if (!sign)std::swap(t1, t2);
	int day1 = daysInYear(t1.year) - dayOfYear(t1);
	int day2 = dayOfYear(t2);
	int dYear = 0;
	for (int i = t1.year + 1; i <= t2.year - 1; i++) {
		dYear += daysInYear(i);
	}
	Duration res;
	res.days = sign * (day1 + day2 + dYear);
	res.seconds = 0;
	return res;
#endif
}

Date Calendar::move(Date t, int days) {

	int calcYear = t.year;
	int mon_idx = mon2idx(t.mon, t.isLeap);
	if (days == 0)return t;
	if (days > 0) {
		days += t.day;
		int dYear = daysInYear(calcYear);
		if (days - t.day + dayOfYear(t) >= dYear) {
			for (; days >= dYear; ) {
				days -= dYear;
				calcYear += 1;
				dYear = daysInYear(calcYear);
			}
			mon_idx = 1;
		}
		int dMonth = daysInMonth(mon_idx);
		if (days >= dMonth) {
			for (; days >= dMonth; mon_idx++) {
				days -= dMonth;
				dMonth = daysInMonth(mon_idx);

			}
		}
		if (days == 0) {
			days = dMonth;
			mon_idx--;
			if (mon_idx == 0)mon_idx = monthsInYear(calcYear);
		}
		t = idx2mon(mon_idx);
		t.day = days;
		t.year = calcYear;
		return t;
	}
	else {
		int dYear = dayOfYear(t);
		days += dYear;
		dYear = 0;
		for (; days <= 0; ) {
			calcYear -= 1;
			dYear = daysInYear(calcYear);
			days += dYear;
		}
		t.year = calcYear;
		t.mon = 1;
		t.day = 1;
		return move(t, days - 1);
	}
}
Range Calendar::xtDaysOfMonth()
{
	return d_ptr->info.xtDaysOfMonth;
}

Range Calendar::xtDaysOfYear()
{
	return d_ptr->info.xtDaysOfYear;
}

Range Calendar::xtMonthsOfYear()
{
	return d_ptr->info.xtMonthsOfYear;
}
