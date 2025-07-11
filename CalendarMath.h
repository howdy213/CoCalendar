//Copyright 2025 howdy213
//the MIT License
//Author: howdy213
#pragma once
#include "astronomy.h"
//С���������ĵ�һ������
static constexpr const char* solar_terms_names[] = {
   "С��", "��", "����", "��ˮ", "�@�U", "����", "����", "�Y��", "����", "С�M", "â�N", "����",
   "С��", "����", "����", "̎��", "��¶", "���", "��¶", "˪��", "����", "Сѩ", "��ѩ", "����"
};

static constexpr const char* month_names[] = {
   "����", "����", "����", "����", "����", "����", "����", "����", "����", "ʮ��", "����", "�D��",
};

static constexpr const char* day_names[] = {
   "��һ", "����", "����", "����", "����", "����", "����", "����", "����", "��ʮ",
   "ʮһ", "ʮ��", "ʮ��", "ʮ��", "ʮ��", "ʮ��", "ʮ��", "ʮ��", "ʮ��", "��ʮ",
   "إһ", "إ��", "إ��", "إ��", "إ��", "إ��", "إ��", "إ��", "إ��", "��ʮ",
};

static constexpr const char* celestial_stems[10] = {
   "��", "��", "��", "��", "��", "��", "��", "��", "��", "��"
};

static constexpr const char* terrestrial_branches[12] = {
   "��", "��", "��", "î", "��", "��", "��", "δ", "��", "��", "��", "��"
};
astronomy::REAL estimate_solar_term(int year, int angle);
astronomy::REAL calc_solar_term(int year, int idx);
astronomy::REAL clamp_degrees(astronomy::REAL d);
astronomy::REAL ecliptic_longitude_diff(astronomy::REAL jd);
astronomy::REAL estimate_new_moon_forward(astronomy::REAL jd);
astronomy::REAL estimate_new_moon_backward(astronomy::REAL jd);
astronomy::REAL calc_new_moon_nearby(astronomy::REAL jd);
astronomy::REAL calc_new_moon_nearby(int year, int month, int day);

// NOTE: 1928�꼰֮ǰ��ũ���ñ����ط�ʱ116��23��E��1929�꿪ʼʹ��120��Eƽ̫��ʱ
static constexpr astronomy::REAL TIMEZONE_BEIJING = 8.0 / 24.0;
// (116+23/60)*4*60=(465+8/15)*60=27932
static constexpr astronomy::REAL TIMEZONE_BEIJING_LOCAL = 27932.0 / 86400.0;

void print_daytime(const Time& t);
void print_daytime_cstb(const Time& t);
int days_offset(const Time& t);
