//Copyright 2025 howdy213
//the MIT License
//Author: howdy213
#include"CalendarDef.h"
#include<iomanip>

Time::Time(const Date& d)
{
	year = d.year;
	mon = d.mon;
	day = d.day;
	hour = Unspecified;
	min = Unspecified;
	sec = Unspecified;
	ms = Unspecified;
}

Time::Time(const Moment& m)
{
	year = Unspecified;
	mon = Unspecified;
	day = Unspecified;
	hour = m.hour;
	min = m.min;
	sec = m.sec;
	ms = m.ms;
}

Time Time::operator=(const Date& d)
{
	year = d.year;
	mon = d.mon;
	day = d.day;
	hour = Unspecified;
	min = Unspecified;
	sec = Unspecified;
	ms = Unspecified;
	return *this;
}

Time Time::operator=(const Moment& m)
{
	year = Unspecified;
	mon = Unspecified;
	day = Unspecified;
	hour = m.hour;
	min = m.min;
	sec = m.sec;
	ms = m.ms;
	return *this;
}

Moment Moment::operator=(const Time& t)
{
	hour = t.hour;
	min = t.min;
	sec = t.sec;
	ms = t.ms;
	return *this;
}
Moment::Moment(const Time& t)
{
	hour = t.hour;
	min = t.min;
	sec = t.sec;
	ms = t.ms;
}
Date Date::operator=(const Time& t) {
	year = t.year;
	mon = t.mon;
	day = t.day;
	return *this;
}

Date::Date(const Time& t)
{
	year = t.year;
	mon = t.mon;
	day = t.day;
}

bool TimeInfo::isValid()
{
	return time.year > 0 ? time.mon > 0 ? time.day > 0 ?
		time.hour != Unspecified ? time.min != Unspecified ? time.sec != Unspecified ?
		time.ms != Unspecified ? 1 : 0 : 0 : 0 : 0 : 0 : 0 : 0;
}
TimeInfo::TimeInfo(const Date& d)
{
	time.year = d.year;
	time.mon = d.mon;
	time.day = d.day;
	time.hour = Unspecified;
	time.min = Unspecified;
	time.sec = Unspecified;
	time.ms = Unspecified;
}
TimeInfo::TimeInfo(const Moment& m)
{
	time.day = Unspecified;
	time.mon = Unspecified;
	time.day = Unspecified;
	time.hour = m.hour;
	time.min = m.min;
	time.sec = m.sec;
	time.ms = m.ms;
}
bool isValid(Moment m)
{
	return m.hour != Unspecified && m.min != Unspecified && m.sec != Unspecified && m.ms != Unspecified
		&& m.hour != Invalid && m.min != Invalid && m.sec != Invalid && m.ms != Invalid;
}
bool isValid(Date d)
{
	return d.day != Unspecified && d.mon != Unspecified && d.day != Unspecified
		&& d.day != Invalid && d.mon != Invalid && d.day != Invalid;
}
bool isValid(int num)
{
	return num != Unspecified && num != Invalid;
}
Duration operator""_day(unsigned long long days)
{
	return { static_cast<long int>(days) };
}
Time operator+(const Time& t, const Duration& d)
{
	Time res = t;
	res.day += d.days;
	res.ms += d.seconds * 1000.0; // 将秒转换为毫秒
	if (res.ms >= 1000.0) {
		res.sec += static_cast<int>(res.ms / 1000);
		res.ms = fmod(res.ms, 1000.0);
	}
	return res;
}
Time operator-(const Time& t, const Duration& d)
{
	Time res = t;
	res.day -= d.days;
	res.ms -= d.seconds * 1000.0; // 将秒转换为毫秒
	if (res.ms < 0) {
		res.sec -= static_cast<int>(-res.ms / 1000) + 1; // 向下取整
		res.ms = fmod(res.ms, 1000.0);
		if (res.ms < 0) res.ms += 1000.0; // 确保毫秒为正
	}
	return res;
}
bool operator==(const Time& t1, const Time& t2)
{
	return t1.year == t2.year && t1.mon == t2.mon && t1.day == t2.day &&
		t1.hour == t2.hour && t1.min == t2.min && t1.sec == t2.sec && t1.ms == t2.ms;
}
bool operator!=(const Time& t1, const Time& t2)
{
	return !(t1 == t2);
}
bool operator<(const Time& t1, const Time& t2)
{
	if (t1.year != t2.year) return t1.year < t2.year;
	if (t1.mon != t2.mon) return t1.mon < t2.mon;
	if (t1.day != t2.day) return t1.day < t2.day;
	if (t1.hour != t2.hour) return t1.hour < t2.hour;
	if (t1.min != t2.min) return t1.min < t2.min;
	if (t1.sec != t2.sec) return t1.sec < t2.sec;
	return t1.ms < t2.ms;
}
bool operator<=(const Time& t1, const Time& t2)
{
	return !(t1 > t2);
}
bool operator>(const Time& t1, const Time& t2)
{
	return t1 >= t2 && t2 != t1;
}
bool operator>=(const Time& t1, const Time& t2)
{
	return !(t1 < t2);
}
std::ostream& operator<<(std::ostream& os, const Time& t)
{
	if (isValid((Date)t)) {
		if (t.year != Unspecified)os << (t.year < 1000 ? "0" : "")
			<< (t.year < 100 ? "0" : "") << (t.year < 10 ? "0" : "") << t.year;
		os << "-";
		if (t.mon != Unspecified)os << (t.mon < 10 ? "0" : "") << t.mon;
		os << "-";
		if (t.day != Unspecified)os << (t.day < 10 ? "0" : "") << t.day;
		os << " ";
	}
	if (isValid((Moment)t)) {
		if (t.hour != Unspecified)os << (t.hour < 10 ? "0" : "") << t.hour;
		os << ":";
		if (t.min != Unspecified)os << (t.min < 10 ? "0" : "") << t.min;
		os << ":";
		if (t.sec != Unspecified)os << (t.sec < 10 ? "0" : "") << t.sec;
		os << ".";
		if (t.ms != Unspecified)os << (t.ms < 100 ? "0" : "") << (t.ms < 10 ? "0" : "") << (int)t.ms;
	}
#if 0
	if ((t.hour != 0 || t.min > 30) && (t.hour != 23 || t.min < 30))os << ' ';
	else os << '*';
#endif
	return os;
}
Time fromParts(int year, int month, int day, int hour, int min, int sec, real ms)
{
	Time t;
	t.year = year;
	t.mon = month;
	t.day = day;
	t.hour = hour;
	t.min = min;
	t.sec = sec;
	t.ms = ms;
	return t;
}
Moment fromParts(int hour, int min, int sec, real ms)
{
	Moment m;
	m.hour = hour;
	m.min = min;
	m.sec = sec;
	m.ms = ms;
	return m;
}
Date fromParts(int year, int month, int day)
{
	Date d;
	d.year = year;
	d.mon = month;
	d.day = day;
	return d;
}
tm toStdTime(Time t) {
	tm t2{};
	t2.tm_year = t.year - 1900;
	t2.tm_mon = t.mon - 1;
	t2.tm_mday = t.day;
	t2.tm_hour = t.hour;
	t2.tm_min = t.min;
	t2.tm_sec = t.sec;
	t2.tm_isdst = -1;
	return t2;
}
Time fromStdTime(tm t) {
	Time t2{};
	t2.year = t.tm_year + 1900;
	t2.mon = t.tm_mon + 1;
	t2.day = t.tm_mday;
	t2.hour = Unspecified;
	t2.min = Unspecified;
	t2.sec = Unspecified;
	t2.ms = Unspecified;
	return t2;
}

