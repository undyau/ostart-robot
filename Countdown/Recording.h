// Recording.h: interface for the CRecording class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECORDING_H__64EA7DEC_764D_481F_858D_12271EEE16F5__INCLUDED_)
#define AFX_RECORDING_H__64EA7DEC_764D_481F_858D_12271EEE16F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "HighTime.h"

class CRecording 
{
public:
	CRecording(CString& a_Name, CString& a_FollowMsgName);
    CRecording(CString& a_Name, bool a_FinishAtTime, CString a_TimingTime);
    CRecording(CString& a_SerisalisedXml);
	virtual ~CRecording();
    virtual CString GetSerialisedXML();

    CString Name() const {return m_Name;};
    virtual CString TimingDesc() const;

    CString TimingTime() const { return m_TimingTime;};
	CString TimingTime(CString& a_TimingTime) { return m_TimingTime = a_TimingTime;};

    bool IsAfterMsg() {return m_AfterMsg;};
	bool IsAfterMsg(bool a_AfterMsg) {return m_AfterMsg = a_AfterMsg;};
    CString FollowMsgName() {return m_FollowMsgName;};
	CString FollowMsgName(CString& a_FollowMsgName) {return m_FollowMsgName = a_FollowMsgName;};
	bool FinishAtTime() const {ASSERT(!m_AfterMsg); return m_FinishAtTime;};
	bool FinishAtTime(bool a_FinishAtTime) {ASSERT(!m_AfterMsg); return m_FinishAtTime = a_FinishAtTime;};
    
	virtual CHighTimeSpan RealDuration(CHighTime a_Time); // default to max duration
    virtual int ImageIndex() = 0;
    virtual float MaxDuration() = 0;
    virtual bool AppendSoundToFile(CHighTime a_Time, CString& a_FileName) = 0;
	virtual bool IsGenerated() = 0;

protected:
    CString m_Name;
    CString m_FollowMsgName;
    CString m_TimingTime; //HHMMSS

    bool m_FinishAtTime;
    bool m_AfterMsg;
};

#endif // !defined(AFX_RECORDING_H__64EA7DEC_764D_481F_858D_12271EEE16F5__INCLUDED_)
