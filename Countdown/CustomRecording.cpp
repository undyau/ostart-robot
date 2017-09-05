// CustomRecording.cpp: implementation of the CCustomRecording class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Countdown.h"
#include "CustomRecording.h"
#include "Wave.h"
#include "utils.h"

extern CCountdownApp theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomRecording::CCustomRecording(CString& a_Name, CString& a_FollowMsgName):
    CRecording(a_Name, a_FollowMsgName)
    {
    }

CCustomRecording::CCustomRecording(CString& a_Name, bool a_FinishAtTime, CString a_TimingTime):
    CRecording(a_Name, a_FinishAtTime, a_TimingTime)
    {
    }

CCustomRecording::CCustomRecording(CString& a_SerialisedXml) : CRecording(a_SerialisedXml)
    {
    }


CCustomRecording::~CCustomRecording()
    {
    }

CString CCustomRecording::GetSerialisedXML()
    {       
    return ToXML("Type", "Custom") +
        CRecording::GetSerialisedXML();
    } 

float CCustomRecording::MaxDuration()
    {
    CWave wave(FileName());
	if (!wave.IsValid())
		{
		AfxMessageBox("Problem with " + this->Name() + ": " + wave.LastError(), MB_ICONEXCLAMATION);
		return 0.0f;
		}
	float result = wave.getDuration();
    return result;
    }

CString CCustomRecording::FileName()
    {
    CString retVal;

    retVal.Format("%s\\%s.wav", theApp.CustomSoundDir(), m_Name);
    return retVal; 
    }

bool CCustomRecording::AppendSoundToFile(CHighTime a_Time, CString& a_FileName)
    {
    return CopyAppendWaveFile(FileName(), a_FileName);
    }

int CCustomRecording::ImageIndex() 
	{
	if (IsAfterMsg())
		return 3;
	else
		return 2;
	}