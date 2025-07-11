//Copyright 2025 howdy213
//the MIT License
//Author: howdy213
#include "ChineseCalendar.h"
#include "GregorianCalendar.h"
using namespace std;

class ChaineseCalendarPrivate {
public:
	// [0]、[1]冬月、[2]腊月、[3]正月...
	//d->solar_terms中[0]/[1]/[2]:上年小雪大雪冬至
	MyDayTime solar_terms[51]{}, new_moons[28]{};
	int CalcYear = Unspecified;
	int leap = 0;
};

ChineseCalendar::ChineseCalendar() : Calendar() {
	d = new ChaineseCalendarPrivate;
	d_ptr->info.name = "Chinese";
	d_ptr->info.method = "astronmy";
	d_ptr->info.isValid = true;
	d_ptr->info.calcMode = ONLY_RECALC;
	d_ptr->info.leapMode = EXTRA_DAY;
	d_ptr->info.hasYearZero = false;
	d_ptr->info.xtDaysOfMonth = { 30,31 };
	d_ptr->info.xtDaysOfYear = { 353,385 };
	d_ptr->info.xtMonthsOfYear = { 12,13 };
	d_ptr->info.ofstDate = fromParts(1900, 1, 1);
}

ChineseCalendar::~ChineseCalendar() {
	delete d;
	d = nullptr;
	Calendar::~Calendar();
}

int ChineseCalendar::dayOfWeek(Date t)
{
	return 0;
}

int ChineseCalendar::dayOfYear(Date t)
{
	if (!::isValid(t))return Invalid;
	int days = 0;
	for (int i = 1; i < t.mon; i++)days += daysInMonth(i, t.year);
	days += t.day;
	return days;
}

int ChineseCalendar::daysInMonth(int mon_idx, int year)
{
	calc(year);
	GregorianCalendar gcal;
	return gcal.diff(firstDayOfCMonth(mon_idx), firstDayOfCMonth(mon_idx + 1)).days;
}

int ChineseCalendar::daysInYear(int year)
{
	calc(year);
	return GregorianCalendar().diff(firstDayOfCMonth(1), lastDayOfCYInGY(year)).days + 1;
}

int ChineseCalendar::monthsInYear(int year)
{
	calc(year);
	return isLeapYear(year) ? 13 : 12;
}

TimeInfo ChineseCalendar::newMoonTime(int mn_idx)
{
	if (mn_idx < 0 || mn_idx>27) {
		return Date();
	}
	TimeInfo res;
	res.attachTo = "Gregorian";
	res.time = d->new_moons[mn_idx].t;
	return res;
}

TimeInfo ChineseCalendar::solarTermTime(int st_idx)
{
	TimeInfo res;
	res.other.insert({ "SolarTermName",solar_terms_names[(st_idx + 1) % 24] });
	res.time = d->solar_terms[st_idx + 4].t;
	return res;
}

std::unordered_map<int, TimeInfo> ChineseCalendar::solarTermsOfMonth(int mon_idx)
{
	std::unordered_map<int, TimeInfo> res;
	int y = d->CalcYear;
	int nm_idx = firstIdxOfNewMoonInCNY();
	while (nm_idx <= lastIdxOfNewMoonInCNY()) {
		auto& mn0 = d->new_moons[nm_idx];
		auto& mn1 = d->new_moons[nm_idx + 1];
		int rindex = raw2CIndexOfMoon(nm_idx);
		nm_idx++;
		if (rindex != mon_idx)continue;
		// 显示节气
		int st_idx = 0;
		while (1) {
			auto& st = d->solar_terms[st_idx];
			// 节气超过本月了
			if (st.ofst >= mn1.ofst) {
				break;
			}
			// 节气落在本月内
			if (st.ofst >= mn0.ofst) {
				int i = (st_idx + 20) % 24;
				i = i == 0 ? 24 : i;
				res.insert({ i, st.t });
			}
			++st_idx;
		}
	}
	return res;
}

void ChineseCalendar::print()
{
	int y = d->CalcYear;
	printf("%d\n", year());

	int nm_idx = 1;
	int st_idx = 0;
	while (nm_idx < 16) {
		auto& mn0 = d->new_moons[nm_idx];
		auto& mn1 = d->new_moons[nm_idx + 1];
		bool major = (mn1.ofst - mn0.ofst) == 30;
		// 按年显示
		if (d->leap == 0 ? (nm_idx < 3 || nm_idx > 14) : (nm_idx < (d->leap >= 4 ? 3 : 4) || nm_idx >(d->leap <= 15 ? 15 : 14))) {
			++nm_idx;
			continue;
		}
		int readable = raw2Month(nm_idx);

		// 数字方式显示月份
		cout << ((d->leap == 0 || nm_idx != d->leap) ? ' ' : '+') << readable << (major ? '+' : '-') << mn0.t << ") ";
		++nm_idx;

		// 显示节气
		while (1) {
			auto& st = d->solar_terms[st_idx];
			// 节气超过本月了
			if (st.ofst >= mn1.ofst) {
				break;
			}
			// 节气落在本月内
			if (st.ofst >= mn0.ofst) {
				cout << solarTermsName(st_idx) << '-' << st.t << ' ';
			}
			++st_idx;
		}

		printf("\n");
	}

}

void ChineseCalendar::printSolarTerms()
{
	printf("solar terms:\n");
	for (auto& st : d->solar_terms) {
		print_daytime(st.t);
		printf("\n");
	}
}

void ChineseCalendar::printNewMoons()
{
	printf("new moons:\n");
	for (int i = 0; i < 28; ++i) {
		print_daytime(d->new_moons[i].t);
		printf("\n");
	}
}

void ChineseCalendar::printSolarTerms(int month, bool isLeap)
{
	int leap = leapMonth(year());
	printSolarTerms(month + (month > leap) + (month == leap && isLeap));
}

void ChineseCalendar::printSolarTerms(int index)
{
	int y = d->CalcYear;
	int nm_idx = firstIdxOfNewMoonInCNY();
	while (nm_idx <= lastIdxOfNewMoonInCNY()) {
		auto& mn0 = d->new_moons[nm_idx];
		auto& mn1 = d->new_moons[nm_idx + 1];
		bool major = (mn1.ofst - mn0.ofst) == 30;
		int rindex = raw2CIndexOfMoon(nm_idx);
		++nm_idx;

		if (rindex != index)continue;
		// 显示节气
		int st_idx = 0;
		while (1) {
			auto& st = d->solar_terms[st_idx];
			// 节气超过本月了
			if (st.ofst >= mn1.ofst) {
				break;
			}
			// 节气落在本月内
			if (st.ofst >= mn0.ofst) {
				printf(" %s (", solar_terms_names[(st_idx + 21) % 24]);
				print_daytime(st.t);
				printf(")");
			}
			++st_idx;
		}
		printf("\n");
	}
}

bool ChineseCalendar::isMajorMonth(int mon_idx)
{
	if (mon_idx<1 || mon_idx>monthsInYear(Unspecified))return false;
	auto& mn0 = d->new_moons[mon_idx];
	auto& mn1 = d->new_moons[mon_idx + 1];
	return (mn1.ofst - mn0.ofst) >= 30;
}

int ChineseCalendar::leapMonth(int year)
{
	if (!::isValid(year))return Invalid;
	calc(year);
	int m = d->leap - firstIdxOfNewMoonInCNY();
	return d->leap > 0 ? m < 14 ? m : Unspecified : Unspecified;
}

bool ChineseCalendar::isLeapYear(int year)
{
	if (!::isValid(year))return Invalid;
	calc(year);
	return d->leap > 0;
}

bool ChineseCalendar::isDateValid(Date t)
{
	if (!::isValid(t))return Invalid;
	if (t.year < 0) return false; // 年份不能为负
	if (t.mon < 1 || t.mon > 12) return false; // 月份必须在1到12之间
	if (t.day < 1 || t.day > daysInMonth(t.mon, t.year)) return false; // 日期必须在该月的有效范围内
	if (t.isLeap == true && t.mon != leapMonth(t.year))return false;
	return true;
}

int ChineseCalendar::offset(Date t)
{
	if (!::isValid(t))return Invalid;
	calc(t.year);
	return d->new_moons[cIndex2RawOfMoon(1)].ofst + dayOfYear(t) - 1;//- 693933;
	//return diff(fromParts(1900, 1, 1), d).days;
}

Duration ChineseCalendar::diff(Date t1, Date t2)
{
	if (!::isValid(t1) || !::isValid(t2))return Duration();
	return { offset(t2) - offset(t1) };
}

Date ChineseCalendar::move(Date t, int days)
{
	return Calendar::move(t, days);
}

int ChineseCalendar::year() {
	return d->CalcYear;
}

void ChineseCalendar::calc(int y)
{
	if (y == Unspecified)return;
	if (d->CalcYear == y) return;
	if (d->CalcYear != Unspecified) {
		clear();
	}
	d->CalcYear = y;
	int st_idx = 2, nm_idx = 1;
	constexpr int WINTER_SOLSTICE_INDEX = 23 - 5;
	astronomy::REAL tz = y >= 1929 ? TIMEZONE_BEIJING : TIMEZONE_BEIJING_LOCAL;

	astronomy::REAL jd_st = calc_solar_term(y - 1, WINTER_SOLSTICE_INDEX);
	astronomy::REAL jd_nm = calc_new_moon_nearby(estimate_new_moon_backward(jd_st));

	d->solar_terms[2].set(jd_st + tz);
	d->new_moons[1].set(jd_nm + tz);

	// 如果朔比冬至大，则说明迭代到下一个月的朔了，需要检查更早一个朔
	if (d->new_moons[1].ofst > d->solar_terms[2].ofst) {
		astronomy::REAL jd_tmp = calc_new_moon_nearby(jd_nm - 29.53);
		d->new_moons[0].set(jd_tmp + tz);
		if (d->new_moons[0].ofst < d->solar_terms[2].ofst) {
			nm_idx = 0;
		}
	}
	else {
		astronomy::REAL jd_tmp = calc_new_moon_nearby(jd_nm + 29.53);
		d->new_moons[2].set(jd_tmp + tz);
		if (d->new_moons[2].ofst == d->solar_terms[2].ofst) {
			d->new_moons[1] = d->new_moons[2];
			jd_nm = jd_tmp;
		}
	}

	// 上年小雪、大雪
	for (int i = 0; i < 2; ++i) {
		jd_st = calc_solar_term(y - 1, (WINTER_SOLSTICE_INDEX + 22 + i) % 24);
		d->solar_terms[i].set(jd_st + tz);
	}

	// 上年冬至~今年冬至
	for (int i = 0; i < 24; ++i) {
		jd_st = calc_solar_term(y, i >= 5 ? i - 5 : i + 19);
		d->solar_terms[i + 3].set(jd_st + tz);
	}

	// 今年冬至~次年冬至
	for (int i = 0; i < 24; ++i) {
		jd_st = calc_solar_term(y + 1, i >= 5 ? i - 5 : i + 19);
		d->solar_terms[i + 27].set(jd_st + tz);
	}

	// 朔
	for (int i = 2; i < 28; ++i) {
		jd_nm = calc_new_moon_nearby(jd_nm + 29.53);
		d->new_moons[i].set(jd_nm + tz);
	}
	// 闰月在上年冬至~今年冬至区间
	// d->solar_terms[26]为今年冬至
	if (d->solar_terms[26].ofst >= d->new_moons[nm_idx + 13].ofst) {
		int ms_idx = 2, o;
		for (int i = 0; i < 13; ++i) {
			o = d->solar_terms[ms_idx].ofst;
			if (d->new_moons[nm_idx + i + 1].ofst <= o) {
				d->leap = nm_idx + i;
				break;
			}
			ms_idx += 2;
		}
	}
	// 闰月在今年冬至~下年冬至区间
	// d->solar_terms[50]为下年冬至
	else if (d->solar_terms[50].ofst >= d->new_moons[nm_idx + 25].ofst) {
		int ms_idx = 26, o;
		for (int i = 0; i < 13; ++i) {
			o = d->solar_terms[ms_idx].ofst;
			if (d->new_moons[nm_idx + i + 13].ofst <= o) {
				d->leap = nm_idx + i + 12;
				break;
			}
			ms_idx += 2;
		}
	}
}

Date ChineseCalendar::idx2mon(int mon_idx)
{
	bool isLeap = false;
	Date res;
	res.mon = cIndex2Month(mon_idx, &isLeap);
	res.isLeap = isLeap;
	return res;
}

int ChineseCalendar::mon2idx(int month, bool isLeap)
{
	return month2CIndex(month, isLeap);
}


Date ChineseCalendar::firstDayOfCMonth(int mon_idx)
{
	return fromDayTime(d->new_moons[mon_idx + firstIdxOfNewMoonInCNY() - 1].t).time;
}

Date ChineseCalendar::lastDayOfCYInGY(int year)
{
	calc(year);
	Time t = d->new_moons[lastIdxOfNewMoonInCNY() + 1].t;
	if (t.day == 1) {
		if (t.mon == 1) {
			t.year = year;
			t.mon = monthsInYear(year);
			t.day = GregorianCalendar().daysInMonth(t.mon, year);
		}
		else {
			t.mon -= 1;
			t.day = GregorianCalendar().daysInMonth(t.mon, year + 1);
			t.year = year + 1;
		}
	}
	else t.day -= 1;
	return t;
}

int ChineseCalendar::firstIdxOfNewMoonInCNY()
{
	return d->leap == 0 ? 3 : (d->leap >= 4 ? 3 : 4);
}

int ChineseCalendar::firstIdxOfNewMoonInGNY()
{
	int i = 0;
	while (d->new_moons[i].t <= fromParts(d->CalcYear, 1, 1, 0, 0, 0, 0)) {
		i++;
	}
	return i;
}

int ChineseCalendar::lastIdxOfNewMoonInCNY()
{
	return d->leap == 0 ? 14 : (d->leap <= 15 ? 15 : 14);
}

int ChineseCalendar::lastIdxOfNewMoonInGNY()
{
	int i = 27;
	while (d->new_moons[i].t >= fromParts(d->CalcYear + 1, 1, 1, 0, 0, 0, 0)) {
		i--;
	}
	return i;
}

std::string ChineseCalendar::monthName(int month, int year)
{
	if (month < 1 || month > 12) {
		return "";
	}
	return month_names[month - 1];
}

std::string ChineseCalendar::weekDayName(int day)
{
	return std::string();
}

std::string ChineseCalendar::solarTermsName(int st_idx)
{
	return solar_terms_names[(st_idx + 21) % 24];
}

std::string ChineseCalendar::format(Date t, const std::string& fmt)
{
	calc(t.year);
	int year = t.year;
	int stem_index = (year - 4) % 10;
	int branch_index = (year - 4) % 12;
	string leap = "";
	if (t.isLeap) {
		if (t.mon == leapMonth(Unspecified))leap = "閏";
	}
	// 组合天干地支
	return std::string(celestial_stems[stem_index]) + terrestrial_branches[branch_index] + monthName(t.mon) + day_names[t.day-1];
}

int ChineseCalendar::cIndex2Month(int rindex, bool* isLeap)
{
	if (isLeap)*isLeap = (rindex == raw2CIndexOfMoon(d->leap));
	return rindex - ((rindex > raw2CIndexOfMoon(d->leap)) && isLeapYear(year()));
}

int ChineseCalendar::month2CIndex(int month, bool isLeap)
{
	bool adjust = (month == raw2CIndexOfMoon(d->leap) && isLeap) || month > raw2CIndexOfMoon(d->leap);
	return month + adjust;
}

int ChineseCalendar::raw2CIndexOfMoon(int index)
{
	return index - 2;
}

int ChineseCalendar::cIndex2RawOfMoon(int rindex)
{
	return rindex + 2;
}

int ChineseCalendar::raw2Month(int index)
{
	int nm_idx = index;
	bool adjust = !(d->leap == 0 || nm_idx < d->leap);
	int month = (nm_idx + 8 + !adjust) % 12 + 1;
	return month;
}

void ChineseCalendar::clear()
{
	d->CalcYear = Unspecified;
	for (int i = 0; i <= 27; i++) {
		d->new_moons[i] = MyDayTime();
	}
	for (int i = 0; i <= 50; i++) {
		d->solar_terms[i] = MyDayTime();
	}

	d->leap = 0;
}

TimeInfo fromDayTime(const Time& t) {
	TimeInfo time;
	time.time = t;
	time.attachTo = "Gregorian";
	return time;
}