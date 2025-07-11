//Copyright 2025 howdy213
//the MIT License
//Author: howdy213
#pragma once
#include "Calendar.h"
class GregorianCalendar :
	public Calendar
{
public:
	GregorianCalendar();
	~GregorianCalendar();

	int dayOfWeek(Date t);
	int daysInMonth(int mon_idx, int year = Unspecified);
	int daysInYear(int year);
	int monthsInYear(int year);

	bool hasYearZero();
	bool isLeapYear(int year);

	bool isDateValid(Date t) override;
	int offset(Date t) override;
	Duration diff(Date t1, Date t2);
	Date move(Date t, int days);

	std::string monthName(int month, int year = Unspecified);
	std::string weekDayName(int day);
	std::string format(Date t, const std::string& fmt) override;
	CALENDAR_FRIEND;
};

