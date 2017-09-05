#pragma once
#include "TimeVal.hpp"
#include "Slot.hpp"
#include <list>

using namespace std;

class CRecord
{
	
public:
	CRecord(string a_Name) {m_Name = a_Name;}
	virtual float DurationForTime(CTimeVal a_Time) = 0;
	virtual list<CSlot> RecordsForTime(CTimeVal) = 0;
	string Name() {return m_Name;}
	
private:
	string m_Name;  // only used for debugging
};
