// CustomRecording.h: interface for the CCustomRecording class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMRECORDING_H__71234F74_1455_4B17_B7EE_4601D64418F2__INCLUDED_)
#define AFX_CUSTOMRECORDING_H__71234F74_1455_4B17_B7EE_4601D64418F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Recording.h"

class CCustomRecording : public CRecording  
{
public:
	CCustomRecording(CString& a_Name, CString& a_FollowMsgName);
    CCustomRecording(CString& a_Name, bool a_FinishAtTime, CString a_TimingTime);
    CCustomRecording(CString& a_SerialisedXml);
	virtual ~CCustomRecording();
    virtual float MaxDuration();
    virtual CString FileName();
    virtual int ImageIndex();
    virtual CString GetSerialisedXML();
	virtual bool IsGenerated() {return false;};

    virtual bool AppendSoundToFile(CHighTime a_Time, CString& a_FileName);

};

#endif // !defined(AFX_CUSTOMRECORDING_H__71234F74_1455_4B17_B7EE_4601D64418F2__INCLUDED_)
