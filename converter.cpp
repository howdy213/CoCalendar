//Copyright 2025 howdy213
//the MIT License
//Author: howdy213
#include"converter.h"
using namespace std;
template<class T>
shared_ptr<T> makePtr(T* ptr) {
	shared_ptr<T> _ptr(ptr ? ptr : new T, ptr ? [](T*) {} : [](T* cal) {delete cal; });
	return _ptr;
};
template<> TimeInfo converter(TimeInfo ti, ChineseCalendar* c1, GregorianCalendar* c2) {
	if (ti.time == TimeInfo().time)return TimeInfo();
	shared_ptr<ChineseCalendar> chc = makePtr(c1);
	shared_ptr<GregorianCalendar> grc = makePtr(c2);

	Time t = ti.time;
	chc->calc(t.year);
	TimeInfo res = grc->move(chc->newMoonTime(chc->firstIdxOfNewMoonInCNY()).time, chc->dayOfYear(t) - 1);

	return res;
}
template<> TimeInfo converter(TimeInfo ti, GregorianCalendar* c1, ChineseCalendar* c2) {
	if (ti.time == TimeInfo().time)return TimeInfo();
	shared_ptr<GregorianCalendar> grc = makePtr(c1);
	shared_ptr<ChineseCalendar> chc = makePtr(c2);

	Time t = ti.time;
	chc->calc(t.year);
	TimeInfo moon_ti = chc->newMoonTime(chc->firstIdxOfNewMoonInCNY());
	TimeInfo res = chc->move(fromParts(t.year, 1, 1), grc->diff(t, moon_ti.time).days);

	return res;
}

