// StartListRecording.h: interface for the CStartListRecording class.
//
//////////////////////////////////////////////////////////////////////
#pragma once


#include "Recording.h"

class CStartListRecording : public CRecording  
{
public:
	CStartListRecording(CString& a_Name, bool a_FinishAtTime, 
                               CString a_TimingTime, 
                               CString a_OffsetTime);
    CStartListRecording(CString& a_Name, CString& a_AfterMsg, 
							   CString a_OffsetTime);
    CStartListRecording(CString& a_SerisalisedXml);	
    virtual CString GetSerialisedXML();

    virtual ~CStartListRecording();
    virtual float MaxDuration();
    virtual int ImageIndex();
    static float FileDuration(CString a_File);
	virtual bool IsGenerated() {return true;};

    virtual CHighTimeSpan RealDuration(CHighTime a_Time);
    virtual bool AppendSoundToFile(CHighTime a_Time, CString& a_FileName);

    static float s_MaxDuration;
	CString OffsetTime() {return m_OffsetTime;}
	CString OffsetTime(CString& a_OffsetTime) {return m_OffsetTime = a_OffsetTime;}


private:
    CString m_CustomDir;
    CString m_NamesDir;
    CString m_DefaultDir;
    CString m_OffsetTime; //MM

    CString FileLocation(CString a_File);
};

