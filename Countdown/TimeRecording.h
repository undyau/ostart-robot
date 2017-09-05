// TimeRecording.h: interface for the CTimeRecording class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMERECORDING_H__A35A7F3F_2719_414A_876C_88407297AE70__INCLUDED_)
#define AFX_TIMERECORDING_H__A35A7F3F_2719_414A_876C_88407297AE70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Recording.h"

class CTimeRecording : public CRecording  
{
public:
	CTimeRecording(CString& a_Name, bool a_FinishAtTime, 
                               CString a_TimingTime, 
                               CString a_OffsetTime);
    CTimeRecording(CString& a_Name, CString& a_AfterMsg, 
							   CString a_OffsetTime);
    CTimeRecording(CString& a_SerisalisedXml);	
    virtual CString GetSerialisedXML();

    virtual ~CTimeRecording();
    virtual float MaxDuration();
    virtual int ImageIndex();
    static float MaxDurationHHMM();
    static float MaxDurationMM();
    static float MaxDurationSS();
    static float FileDuration(CString a_File);
	virtual bool IsGenerated() {return true;};

    virtual CHighTimeSpan RealDuration(CHighTime a_Time);
    virtual bool AppendSoundToFile(CHighTime a_Time, CString& a_FileName);

    static float s_MaxDurationHHMM;
    static float s_MaxDurationMM;
	CString OffsetTime() {return m_OffsetTime;}
	CString OffsetTime(CString& a_OffsetTime) {return m_OffsetTime = a_OffsetTime;}


private:
    CString m_CustomDir;
    CString m_DefaultDir;
    CString m_OffsetTime; //MM

    CString FileLocation(CString a_File);
    void Init();
};

#endif // !defined(AFX_TIMERECORDING_H__A35A7F3F_2719_414A_876C_88407297AE70__INCLUDED_)
