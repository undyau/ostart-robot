#include "TimeVal.hpp"
#include "time.h"
#include <iostream>
using namespace std;

CTimeVal::CTimeVal(timeval a_Time) : m_Time(a_Time)
{
}

CTimeVal::CTimeVal()
{
	gettimeofday(&m_Time, NULL);
	
/*	time_t rawtime;
	struct tm * timeinfo;

	time (&rawtime);
	timeinfo = localtime (&rawtime);
	cout << "TimeVal set at " << timeinfo->tm_hour << ":" << timeinfo->tm_min << ":" << timeinfo->tm_sec << endl;	
	cout << "TimeVal set to " << TimeString() << endl;	*/
}

  
float CTimeVal::operator-(const CTimeVal& a_Rhs)  
{
	CTimeVal rhs(a_Rhs);
	float result = m_Time.tv_sec - rhs.m_Time.tv_sec;
	result += ((float)(m_Time.tv_usec - rhs.m_Time.tv_usec))/1000000;
	return result;
}	

CTimeVal CTimeVal::operator+ (const CTimeVal& a_Rhs)  
{
	CTimeVal rhs(a_Rhs);
	CTimeVal result(m_Time);
	result.m_Time.tv_sec += a_Rhs.m_Time.tv_sec;
	result.m_Time.tv_usec += a_Rhs.m_Time.tv_usec;
	
	if (result.m_Time.tv_usec > 1000000)
		{
		result.m_Time.tv_sec++;
		result.m_Time.tv_usec -= 1000000;
		}

	if (result.m_Time.tv_usec < 0)
		{
		result.m_Time.tv_sec--;
		result.m_Time.tv_usec += 1000000;
		}

	return result;
}


bool CTimeVal::operator<(const CTimeVal& a_Rhs) const
{
	if (a_Rhs.m_Time.tv_sec != m_Time.tv_sec)
		return  m_Time.tv_sec < a_Rhs.m_Time.tv_sec;
		
	return m_Time.tv_usec < a_Rhs.m_Time.tv_usec;
}

int CTimeVal::Hour() const
{
  time_t t = (time_t)m_Time.tv_sec;
  struct tm* time = localtime(&t);
  return time->tm_hour;
}

int CTimeVal::Min() const
{
  time_t t = (time_t)m_Time.tv_sec;
  struct tm* time = localtime(&t);
  return time->tm_min;
}

int CTimeVal::Sec() const
{
  time_t t = (time_t)m_Time.tv_sec;
  struct tm* time = localtime(&t);
  return time->tm_sec;
}

int CTimeVal::MilliSec() const
{
  return (m_Time.tv_usec /1000);
}



void CTimeVal::AddSecs(int a_Secs)
{
	m_Time.tv_sec += a_Secs;
}

void CTimeVal::AddSecs(float a_Secs)
{
	a_Secs *= 1000000;
	long int microsecs = (long int)a_Secs;
	AddMicroSecs(microsecs);
}

void CTimeVal::AddMicroSecs(long int a_MicroSecs)
{
	while (m_Time.tv_usec + a_MicroSecs >= 1000000)
		{
		m_Time.tv_sec++;
		a_MicroSecs -= 1000000;
		}
	while (m_Time.tv_usec + a_MicroSecs < 0)
		{
		m_Time.tv_sec--;
		a_MicroSecs += 1000000;
		}

	m_Time.tv_usec += a_MicroSecs;
}

unsigned int CTimeVal::ToUnsignedInt()
{
	return (unsigned int) (m_Time.tv_usec + (1000000 * m_Time.tv_sec));
}

string CTimeVal::TimeString() const
{
	char buf[256];
	sprintf(buf, "%d:%.02d:%.02d.%d", Hour(), Min(), Sec(), MilliSec());
	string retVal(buf);
	return retVal;
}

void CTimeVal::Set(timeval t)
{
	m_Time = t;
}
