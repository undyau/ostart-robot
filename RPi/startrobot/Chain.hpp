#pragma once
#include "Record.hpp"
#include <list>
#include "TimeVal.hpp"
#include "Slot.hpp"
#include <memory>
#include "Chain.hpp"

using namespace std;

class CChain
{
public:
	CChain(bool a_AnchoredStart);
	void AddRecord(std::unique_ptr<CRecord> a_Rec);
	bool AnchoredStart() {return m_AnchoredStart;}
	string RecordsForTimeAsString(CTimeVal a_TimeVal);	
	float DurationForTime(CTimeVal a_Time);	
	
private:
	std::list<unique_ptr<CRecord>> m_Records;
	bool m_AnchoredStart; // Either start is anchored or finish is anchored
	std::list<CSlot> RecordsForTime(CTimeVal a_TimeVal);
};
