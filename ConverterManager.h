//Copyright 2025 howdy213
//the MIT License
//Author: howdy213
#pragma once
#include"Calendar.h"
#include"CalendarDef.h"
#include<map>
#include<vector>
constexpr char fromTypeStr[] = "fromType";
constexpr char toTypeStr[] = "toType";
using ConverterFunc = TimeInfo(*)(TimeInfo, Calendar*, Calendar*);
struct FromTo {
	std::string from;
	std::string to;
	ConverterFunc func;
};
#define CVT_ADD_DEFAULT_INFO(ti) do{ti.other.insert({fromTypeStr,typeid(c1).name()});ti.other.insert({toTypeStr,typeid(c2).name()});}while(0)
class ConverterManager
{
public:
	static ConverterManager* inst();
	template<class from, class to >
	bool regConverter();
	template<class cal>
	bool regCalendar();
	bool unregCalendar(std::string name);
	TimeInfo convert(TimeInfo ti, std::string fromName, std::string toName, Calendar* c1 = nullptr, Calendar* c2 = nullptr);
	TimeInfo convert(TimeInfo ti, Calendar* c1, Calendar* c2);
private:
	static ConverterManager* instance;
	std::map<std::string, Calendar*>calInst;
	std::vector<FromTo>cvt;
};

using namespace std;
template<class from, class to>
bool ConverterManager::regConverter()
{
	TimeInfo ti = converter<from, to>(TimeInfo());
	if (ti.other.find("NotDefined") != ti.other.end())return false;

	auto lam = [](TimeInfo ti, Calendar* c1, Calendar* c2)->TimeInfo { return converter<from, to>(ti, (from*)c1, (to*)c2); };
	ConverterFunc f = lam;
	FromTo ft = { from().name(),to().name(),f };
	cvt.push_back(ft);
	return true;
}

template<class cal>
bool ConverterManager::regCalendar()
{
	static_assert(!is_base_of<Calendar, cal>::value, "ConverterManager::regCalendar() receives wrong type");
	cal c;
	string name = c.name();
	instance.insert("");
	return false;
}

