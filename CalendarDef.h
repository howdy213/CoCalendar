//Copyright 2025 howdy213
//the MIT License
//Author: howdy213
#pragma once
#include<ctime>
#include<iostream>
#include<string>
#include<unordered_map>
#include <variant>
#define AUTO_REG 1
namespace cc {
}
struct Duration;
class Date;
class Moment;
class Time;
class TimeInfo;
class Locale;
using real = long double;
const int Invalid = INT32_MAX;
const int Unspecified = INT32_MAX - 1;
class Calendar;
class CalendarInfo;
#define CALENDAR_FRIEND template<typename from, typename to>friend TimeInfo converter(TimeInfo t,from* c1, to* c2)
template<typename from, typename to>TimeInfo converter(TimeInfo ti, from* c1 = nullptr, to* c2 = nullptr);

class Moment {
public:
	int hour = Unspecified;
	int min = Unspecified;
	int sec = Unspecified;
	real ms = Unspecified;
	Moment() = default;
	Moment operator = (const Time& t);
	Moment(const Time& t);
};

class Date {
public:
	int year = Unspecified;
	int mon = Unspecified;
	int day = Unspecified;
	bool isLeap = false;
	Date() = default;
	Date operator = (const Time& t);
	Date(const Time& t);
};

class Time {
public:
	int year = Unspecified;
	int mon = Unspecified;
	int day = Unspecified;
	int hour = Unspecified;
	int min = Unspecified;
	int sec = Unspecified;
	real ms = Unspecified;
	Time() = default;
	Time(const Date& d);
	Time(const Moment& m);
	Time operator = (const Date& d);
	Time operator = (const Moment& m);
};

class TimeInfo {
public:
	bool isValid();
	TimeInfo() = default;
	TimeInfo(const Date& d);
	TimeInfo(const Moment& m);
	TimeInfo(const Time& t) : time(t) {}
	Time time;
	//Locale locale;
	std::string attachTo;
	std::unordered_map<std::string, std::variant<int, std::string>> other{};  // 扩展字段
};
/// <summary>
/// days 正负代表时间段方向，seconds恒为正
/// </summary>
struct Duration {
	long int days = 0;
	real seconds = 0.0;
};

bool isValid(Moment m);
bool isValid(Date d);
bool isValid(int num);
Duration operator"" _day(unsigned long long days);

Time operator+(const Time& t, const Duration& d);
Time operator-(const Time& t, const Duration& d);
bool operator==(const Time& t1, const Time& t2);
bool operator!=(const Time& t1, const Time& t2);
bool operator<(const Time& t1, const Time& t2);
bool operator<=(const Time& t1, const Time& t2);
bool operator>(const Time& t1, const Time& t2);
bool operator>=(const Time& t1, const Time& t2);

std::ostream& operator<<(std::ostream& os, const Time& t);

Time fromParts(int year, int month, int day, int hour, int min, int sec, real ms);
Moment fromParts(int hour, int min, int sec, real ms);
Date fromParts(int year, int month, int day);

tm toStdTime(Time t);
Time fromStdTime(tm t);

enum CalcMode { ONLY_RECALC, SPT_MOVE };
enum LeapMode { NONE, EXTRA_DAY, EXTRA_MONTH, EXTRA_BOTH };
struct Range {
	int min = Unspecified;
	int max = Unspecified;
};

class CalendarInfo {
public:
	std::string name;
	std::string method = "default";
	bool isValid = false;
	bool hasYearZero = false;
	LeapMode leapMode = NONE;
	CalcMode calcMode = ONLY_RECALC;
	Date ofstDate = fromParts(1, 1, 1);
	Range xtDaysOfMonth{ 28, 31 };
	Range xtDaysOfYear{ 365, 366 };
	Range xtMonthsOfYear{ 12, 12 };
};

template<typename from, typename to>
inline TimeInfo converter(TimeInfo ti, from* c1, to* c2)
{
	ti.other.insert({"NotDefined",""});
	return ti;
}