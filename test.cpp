//Copyright 2025 howdy213
//the MIT License
//Author: howdy213
#include "astronomy.h"
#include"AstronomyTest.h"
#include "Calendar.h"
#include "ChineseCalendar.h"
#include"converter.h"
#include"ConverterManager.h"
#include "GregorianCalendar.h"
#include <iostream>
using namespace std;
void test() {
	// 测试日期范围
	Date t1 = fromParts(2025, 1, 1);
	Date t2 = fromParts(2026, 12, 29);
	Date t3 = fromParts(2026, 12, 30);
	GregorianCalendar gcal;
	ChineseCalendar ccal;
	cout << "测试日期:" << endl;
	cout << "Date1------------------------------- " << t1 << endl;
	cout << "Date2------------------------------- " << t2 << endl;
	cout << "Date3------------------------------- " << t3 << endl << endl;

	// 公历(Gregorian Calendar)测试
	cout << "========== 公历(Gregorian Calendar)测试 ==========" << endl;
	cout << "Calendar Name----------------------- " << gcal.name() << endl;
	cout << "Formatted Date1--------------------- " << gcal.format(t1, "YYMMDD") << endl;
	cout << "Is Date1 Valid---------------------- " << (gcal.isDateValid(t1) ? "Yes" : "No") << endl;
	cout << "Date1 Weekday----------------------- " << gcal.weekDayName(gcal.dayOfWeek(t1)) << endl;
	cout << "Difference in days------------------ " << gcal.diff(t1, t2).days << endl;
	cout << "Offset of Date1--------------------- " << gcal.offset(t1) << endl;
	cout << "Move Date1 by 10 days--------------- " << gcal.move(t1, 10) << endl << endl;

	// 农历(Chinese Calendar)测试
	cout << "========== 农历(Chinese Calendar)测试 ==========" << endl;
	cout << "Calendar Name----------------------- " << ccal.name() << endl;
	cout << "Formatted Date---------------------- " << ccal.format(t1, "YYYY/MM/DD") << endl;
	cout << "Is Date1 Valid---------------------- " << (ccal.isDateValid(t1) ? "Yes" : "No") << endl;
	cout << "Is Date3 Valid---------------------- " << (ccal.isDateValid(t3) ? "Yes" : "No") << endl;
	cout << "Leap Month of 2025------------------ " << ccal.leapMonth(2025) << endl;

	// 节气测试
	cout << "Print Solar Terms In First Month---- ";
	auto m = ccal.solarTermsOfMonth(1);
	for (auto& term : m) {
		cout << term.first << " : ";
		cout << term.second.time << " ";
	}
	cout << endl;

	// 新月测试
	cout << "Print First New Moon---------------- ";
	cout << ccal.newMoonTime(ccal.firstIdxOfNewMoonInCNY()).time << endl;

	// 日期计算测试
	cout << "Difference in days------------------ " << ccal.diff(t1, t2).days << endl;
	cout << "Offset of Date1--------------------- " << ccal.offset(t1) << endl;
	cout << "Move Date1 by 10 days--------------- " << ccal.move(t1, 10) << endl << endl;

	// 日历转换测试
	cout << "========== 日历转换测试 ==========" << endl;
	Date cvt = ConverterManager::inst()->convert(t1, &ccal, &gcal).time;
	cout << "Convert Date1 From ccal to gcal----- " << cvt << endl;
	cout << "Convert Back to ccal---------------- " << converter<GregorianCalendar, ChineseCalendar>(cvt).time << endl << endl;

	// 2025年农历详细计算
	cout << "========== 2025年农历详细计算 ==========" << endl;
	cout << "Complete Chinese Calendar Calculation for 2025:" << endl;
	calc_chn_cal(2025);
	cout << endl;

	// 每月详情
	cout << "========== 2025年农历每月详情 ==========" << endl;
	for (int i = 1; i <= 13; ++i) {
		cout << "Month " << i << ": " << ccal.firstDayOfCMonth(i) << " ~ ";
		auto m = ccal.solarTermsOfMonth(i);
		for (auto& term : m) {
			cout << term.first << " : ";
			cout << term.second.time << " ";
		}
		cout << "Total:" << ccal.daysInMonth(i, Unspecified) << " days" << endl;
	}
	cout << "Total days in 2025: " << ccal.daysInYear(2025) << " days" << endl;
}
int main() {
	test();
	return 0;
}
