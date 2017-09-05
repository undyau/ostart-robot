// Recording.cpp: implementation of the CRecording class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Countdown.h"
#include "Recording.h"
#include "utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecording::CRecording(CString& a_Name, CString& a_FollowMsgName):
    m_Name(a_Name), m_FollowMsgName(a_FollowMsgName), 
        m_TimingTime(_T("00:00.0")), m_AfterMsg(true)
    {    
    }

CRecording::CRecording(CString& a_Name, bool a_FinishAtTime, CString a_TimingTime):
    m_Name(a_Name), m_FinishAtTime(a_FinishAtTime), m_TimingTime(a_TimingTime), 
        m_AfterMsg(false)
    {
    }

CRecording::CRecording(CString& a_SerialisedXML)
    {
    #define VARBITS(a) #a, a

    GetXMLValue(a_SerialisedXML, VARBITS(m_Name));
    GetXMLValue(a_SerialisedXML, VARBITS(m_FollowMsgName));
    //GetXMLValue(a_SerialisedXML, VARBITS(m_FreqTime)); 
    GetXMLValue(a_SerialisedXML, VARBITS(m_TimingTime));  

    GetXMLValue(a_SerialisedXML, VARBITS(m_FinishAtTime));
    GetXMLValue(a_SerialisedXML, VARBITS(m_AfterMsg));

    #undef VARBITS
    }

CRecording::~CRecording()
    {
    }

CString CRecording::TimingDesc() const
    {
    if (m_AfterMsg)
        {
        CString gap = m_TimingTime == "00:00:00.0" ? "" :  m_TimingTime + " ";
        return gap + "after " + m_FollowMsgName;
        }
    else
        {
        CString when = m_TimingTime == "00:00:00.0" ? "" :  m_TimingTime + " ";
        if (m_FinishAtTime)
            return "finishing " + when + " after the schedule time";
        else
            return "starting " + when + " after the schedule time";        
        }
    }

CHighTimeSpan CRecording::RealDuration(CHighTime a_Time)
	{
	return FloatSecsToSpan(MaxDuration());
	}

CString CRecording::GetSerialisedXML()
    {
#define VARBITS(a) #a, a
    
    return ToXML(VARBITS(m_Name)) +
           ToXML(VARBITS(m_FollowMsgName)) +
           //ToXML(VARBITS(m_FreqTime)) +
           ToXML(VARBITS(m_TimingTime)) +
           ToXML(VARBITS(m_FinishAtTime)) +
           ToXML(VARBITS(m_AfterMsg));
#undef VARBITS
    }

