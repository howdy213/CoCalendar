//Copyright 2025 howdy213
//the MIT License
//Author: howdy213
#pragma once
#include "astronomy.h"
//小寒：公历的第一个节气
static constexpr const char* solar_terms_names[] = {
   "小寒", "大寒", "立春", "雨水", "驚蟄", "春分", "清明", "穀雨", "立夏", "小滿", "芒種", "夏至",
   "小暑", "大暑", "立秋", "處暑", "白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪", "冬至"
};

static constexpr const char* month_names[] = {
   "正月", "二月", "三月", "四月", "五月", "六月", "七月", "八月", "九月", "十月", "冬月", "臘月",
};

static constexpr const char* day_names[] = {
   "初一", "初二", "初三", "初四", "初五", "初六", "初七", "初八", "初九", "初十",
   "十一", "十二", "十三", "十四", "十五", "十六", "十七", "十八", "十九", "二十",
   "廿一", "廿二", "廿三", "廿四", "廿五", "廿六", "廿七", "廿八", "廿九", "三十",
};

static constexpr const char* celestial_stems[10] = {
   "甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"
};

static constexpr const char* terrestrial_branches[12] = {
   "子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"
};
astronomy::REAL estimate_solar_term(int year, int angle);
astronomy::REAL calc_solar_term(int year, int idx);
astronomy::REAL clamp_degrees(astronomy::REAL d);
astronomy::REAL ecliptic_longitude_diff(astronomy::REAL jd);
astronomy::REAL estimate_new_moon_forward(astronomy::REAL jd);
astronomy::REAL estimate_new_moon_backward(astronomy::REAL jd);
astronomy::REAL calc_new_moon_nearby(astronomy::REAL jd);
astronomy::REAL calc_new_moon_nearby(int year, int month, int day);

// NOTE: 1928年及之前的农历用北京地方时116°23′E，1929年开始使用120°E平太阳时
static constexpr astronomy::REAL TIMEZONE_BEIJING = 8.0 / 24.0;
// (116+23/60)*4*60=(465+8/15)*60=27932
static constexpr astronomy::REAL TIMEZONE_BEIJING_LOCAL = 27932.0 / 86400.0;

void print_daytime(const Time& t);
void print_daytime_cstb(const Time& t);
int days_offset(const Time& t);
