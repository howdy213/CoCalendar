//Copyright 2025 howdy213
//the MIT License
//Author: howdy213
#pragma once
#include "astronomy.h"
#include "Calendar.h"
#include "CalendarMath.h"

TimeInfo fromDayTime(const Time& t);

struct MyDayTime {
	Time t;
	int ofst = Unspecified;

	void set(astronomy::REAL jd) {
		astronomy::daytime_from_julian_day(jd - astronomy::calc_delta_t(jd), &t);
		ofst = days_offset(t);
	}
};


class ChaineseCalendarPrivate;
class ChineseCalendar :
	public Calendar
{
public:
	ChineseCalendar();
	~ChineseCalendar();
	void clear() override;

	int dayOfWeek(Date t) override;
	int dayOfYear(Date t) override;
	int daysInMonth(int mon_idx, int year = Unspecified) override;
	int daysInYear(int year) override;
	int monthsInYear(int year) override;

	bool isMajorMonth(int mon_idx);
	bool isLeapYear(int year) override;
	int leapMonth(int year) override;
	Date idx2mon(int mon_idx);
	int mon2idx(int month, bool isLeap = false);

	bool isDateValid(Date t) override;
	int offset(Date t) override;
	Duration diff(Date t1, Date t2) override;
	Date move(Date t, int days);

	int year();
	void calc(int y);

	TimeInfo newMoonTime(int mn_idx);
	TimeInfo solarTermTime(int st_idx);
	std::unordered_map<int, TimeInfo> solarTermsOfMonth(int mon_idx);

	//idx:1~12/13->mon:1~12
	Date firstDayOfCMonth(int mon_idx);
	Date lastDayOfCYInGY(int year);
	int firstIdxOfNewMoonInCNY();
	int firstIdxOfNewMoonInGNY();
	int lastIdxOfNewMoonInCNY();
	int lastIdxOfNewMoonInGNY();

	std::string monthName(int month, int year = Unspecified)  override;
	std::string weekDayName(int day)  override;
	std::string solarTermsName(int st_idx);
	std::string format(Date t, const  std::string& fmt) override;

	void print();
	void printSolarTerms();
	void printNewMoons();
	void printSolarTerms(int month, bool isLeap);
	void printSolarTerms(int index);

private:
	CALENDAR_FRIEND;
	int cIndex2Month(int rindex, bool* isLeap = nullptr);
	int month2CIndex(int month, bool isLeap = false);
	int raw2CIndexOfMoon(int index);
	int cIndex2RawOfMoon(int rindex);
	int raw2Month(int index);
	ChaineseCalendarPrivate* d = nullptr;
};