#include "Record.hpp"
#include "TimeVal.hpp"
#include "Slot.hpp"

class CStartListRecord : public CRecord
{
public:
	CStartListRecord(string a_Name, int a_MinOffset);
	virtual float DurationForTime(CTimeVal a_Time);
	virtual std::list<CSlot> RecordsForTime(CTimeVal a_Time);
	
private:
	int m_MinOffset;  // 0 = next minute, -1 = current (just started) minute
	std::list<string> TitlesForTime(CTimeVal a_Time);
};
