//Copyright 2025 howdy213
//the MIT License
//Author: howdy213
#pragma once
#include"CalendarDef.h"
#include<string>
#include<unordered_map>
#include<vector>
class Calendar;
class CalendarPrivate {
public:
	CalendarInfo info;
};
class Calendar {
public:
	Calendar();
	~Calendar();

	virtual std::string name();
	virtual std::string method();
	virtual void clear();
	virtual bool isValid();
	virtual LeapMode leapMode();
	virtual CalcMode calcMode();

	virtual int dayOfWeek(Date t) = 0;
	virtual int dayOfYear(Date t);
	virtual int daysInMonth(int mon_idx, int year = Unspecified) = 0;
	virtual int daysInYear(int year) = 0;
	virtual int monthsInYear(int year);

	virtual bool hasYearZero();
	virtual bool isLeapYear(int year) = 0;
	virtual int leapMonth(int year);
	virtual Date idx2mon(int mon_idx);
	virtual int mon2idx(int month, bool isLeap = false);

	virtual bool isDateValid(Date t);
	virtual int offset(Date t);
	virtual Date offsetStd(Date t);
	virtual Duration diff(Date t1, Date t2);
	virtual Date move(Date t, int days);

	virtual Range xtDaysOfMonth();
	virtual Range xtDaysOfYear();
	virtual Range xtMonthsOfYear();

	virtual std::string monthName(int month, int year = Unspecified) = 0;
	virtual std::string weekDayName(int day) = 0;
	virtual std::string format(Date t, const  std::string& fmt) = 0;

protected:
	CalendarPrivate* d_ptr = nullptr;
};