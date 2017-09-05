#include "Record.hpp"
#include "TimeVal.hpp"
#include "Slot.hpp"
#include <list>
#include <string>

class CAnnouncementRecord : public CRecord
{
public:
	CAnnouncementRecord(string a_Name);
	virtual float DurationForTime(CTimeVal a_Time);
	virtual std::list<CSlot> RecordsForTime(CTimeVal a_Time);

private:

};
