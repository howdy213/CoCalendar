//Copyright 2025 howdy213
//the MIT License
//Author: howdy213
#pragma once
#include"CalendarDef.h"
#include"ConverterManager.h"
#include"ChineseCalendar.h"
#include"GregorianCalendar.h"
#include<memory>
#define DECLARE_CONVERTER(from,to)\
template<> TimeInfo converter(TimeInfo ti, from* c1, to* c2); \
static bool from##2##to##Reg = ConverterManager::inst()->regConverter<from, to>(); 

DECLARE_CONVERTER(ChineseCalendar, GregorianCalendar);
DECLARE_CONVERTER(GregorianCalendar, ChineseCalendar);