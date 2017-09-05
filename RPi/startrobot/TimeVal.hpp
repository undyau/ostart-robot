#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <string>
#pragma once

class CTimeVal
{
public:
	CTimeVal(timeval a_Time);
	CTimeVal();
	operator timeval() {return m_Time;};
	//operator int();
	bool operator < (const CTimeVal& a_TimeVal) const;
	float operator - (const CTimeVal& a_Rhs); 
	CTimeVal operator + (const CTimeVal& a_Rhs); 
	void Set(timeval);
	int Hour() const;
	int Min() const;
	int Sec() const;
	int MilliSec() const;
	std::string TimeString() const;
	void AddSecs(int a_Secs);
	void AddSecs(float a_Secs);
	void AddMicroSecs(long int a_MicroSecs);
	unsigned int ToUnsignedInt();

private:
	timeval m_Time;

};
