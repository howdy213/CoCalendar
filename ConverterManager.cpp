//Copyright 2025 howdy213
//the MIT License
//Author: howdy213
#include "ConverterManager.h"
#include<iostream>
ConverterManager* ConverterManager::instance =nullptr;
ConverterManager* ConverterManager::inst()
{
	if (!instance)instance = new ConverterManager;
	return instance;
}

bool ConverterManager::unregCalendar(std::string name)
{
	return false;
}
//TODO:建立图并自动查找路径
TimeInfo ConverterManager::convert(TimeInfo ti, std::string fromName, std::string toName, Calendar* c1, Calendar* c2)
{
	for (auto it = cvt.begin(); it != cvt.end(); it++) {
		if (fromName == it->from && toName == it->to)return it->func(ti, c1, c2);
	}
	return TimeInfo();
}

TimeInfo ConverterManager::convert(TimeInfo ti, Calendar* c1, Calendar* c2)
{
	for (auto it = cvt.begin(); it != cvt.end(); it++) {
		if (c1->name() == it->from && c2->name() == it->to)return it->func(ti,c1,c2);
	}
	return TimeInfo();
}
