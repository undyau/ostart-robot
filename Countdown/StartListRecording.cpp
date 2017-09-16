// StartListRecording.cpp: implementation of the CStartListRecording class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Countdown.h"
#include "StartListRecording.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "Wave.h"
#include "utils.h"
extern CCountdownApp theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


float CStartListRecording::s_MaxDuration= 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStartListRecording::CStartListRecording(CString& a_Name, bool a_FinishAtTime, 
                               CString a_TimingTime, CString a_OffsetTime) : 
    CRecording(a_Name, a_FinishAtTime,  a_TimingTime),
    m_OffsetTime(a_OffsetTime)
    {
    }

CStartListRecording::CStartListRecording(CString& a_Name, CString& a_AfterMsg, 
                               CString a_OffsetTime): 
    CRecording(a_Name, a_AfterMsg), 
    m_OffsetTime(a_OffsetTime)
    {
    }

CStartListRecording::CStartListRecording(CString& a_SerialisedXml) : CRecording(a_SerialisedXml)
    {
    #define VARBITS(a) #a, a
    GetXMLValue(a_SerialisedXml, VARBITS(m_OffsetTime));

    #undef VARBITS
    }

CString CStartListRecording::GetSerialisedXML()
    {    
    #define VARBITS(a) #a, a

    return ToXML("Type", "StartList") +
        CRecording::GetSerialisedXML() +  
        ToXML(VARBITS(m_OffsetTime));

    #undef VARBITS    
    }

CStartListRecording::~CStartListRecording()
    {
    }

float CStartListRecording::MaxDuration()
	{
	if (CStartListRecording::s_MaxDuration != 0)
		return CStartListRecording::s_MaxDuration;

	// Check out each minute in the startlist
	std::vector<CHighTime> starts = theApp.StartList().StartTimes();
	float timeMax(0);
	for (auto i = starts.begin(); i != starts.end(); i++)
		{
		// Sum the time for each name + gaps
		float timeTotal(0);
		std::vector<CString> names = theApp.StartList().StartersAtTime(*i);
		CString name;
		for (auto j = names.begin(); j != names.end(); j++)
			{
			name = theApp.StartList().FileNameOfName(*j);
			if (!FileExists(name))
				throw "Couldn't find the sound file " + name;
			CWave temp(name);
			if (!temp.IsValid()) throw temp.LastError();
			timeTotal += temp.getDuration();
			}
		if (names.size())
			timeTotal += (names.size() - 1) * BIG_GAP_SIZE;
		timeMax = max(timeTotal, timeMax);
		}
	CStartListRecording::s_MaxDuration = timeMax;

	return timeMax;
	}


CHighTimeSpan CStartListRecording::RealDuration(CHighTime a_Time)
    {
    CHighTimeSpan retVal;    
    CHighTimeSpan offset = MM2Span(m_OffsetTime);
	CHighTime startTime = a_Time + offset;

	float timeTotal(0);
	std::vector<CString> names = theApp.StartList().StartersAtTime(startTime);
	CString name;
	for (auto j = names.begin(); j != names.end(); j++)
		{
		name = theApp.StartList().FileNameOfName(*j);
		if (!FileExists(name))
			throw "Couldn't find the sound file " + name;
		CWave temp(name);
		if (!temp.IsValid()) throw temp.LastError();
		timeTotal += temp.getDuration();
		}
	if (names.size())
		timeTotal += (names.size() - 1) * BIG_GAP_SIZE;
	retVal = FloatSecsToSpan(timeTotal);

    return retVal;
    }


bool CStartListRecording::AppendSoundToFile(CHighTime a_Time, CString& a_FileName)
    {
    CHighTimeSpan offset = MM2Span(m_OffsetTime);
	CHighTime startTime = a_Time + offset;

	std::vector<CString> names = theApp.StartList().StartersAtTime(startTime);
	CString name;

	unsigned int i(0);
	for (auto j = names.begin(); j != names.end(); j++)
		{
		name = theApp.StartList().FileNameOfName(*j);
		if (!FileExists(name))
			throw "Couldn't find the sound file " + name;

		if (CopyAppendWaveFile(name, a_FileName))
			{
			if (++i < names.size())
				{
				CWave wave(a_FileName);
				if (!wave.IsValid()) throw wave.LastError();
				wave.AddBlank((float)BIG_GAP_SIZE);
				wave.saveToFile();
				}
			}
		else
			return false;
		}
	return true;
    }

int CStartListRecording::ImageIndex() 
	{
	if (IsAfterMsg())
		return 4;
	else
		return 0;
	}