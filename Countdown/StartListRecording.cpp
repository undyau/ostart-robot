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


float CStartListRecording::s_MaxDurationHHMM = 0;
float CStartListRecording::s_MaxDurationMM = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStartListRecording::CStartListRecording(CString& a_Name, bool a_FinishAtTime, 
                               CString a_TimingTime, CString a_OffsetTime) : 
    CRecording(a_Name, a_FinishAtTime,  a_TimingTime),
    m_OffsetTime(a_OffsetTime)
    {
    Init();
    }

CStartListRecording::CStartListRecording(CString& a_Name, CString& a_AfterMsg, 
                               CString a_OffsetTime): 
    CRecording(a_Name, a_AfterMsg), 
    m_OffsetTime(a_OffsetTime)
    {
    Init();
    }

CStartListRecording::CStartListRecording(CString& a_SerialisedXml) : CRecording(a_SerialisedXml)
    {
    #define VARBITS(a) #a, a
    GetXMLValue(a_SerialisedXml, VARBITS(m_OffsetTime));

    #undef VARBITS
    Init();
    }

CString CStartListRecording::GetSerialisedXML()
    {    
    #define VARBITS(a) #a, a

    return ToXML("Type", "Timed") +
        CRecording::GetSerialisedXML() +  
        ToXML(VARBITS(m_OffsetTime));

    #undef VARBITS    
    }

void CStartListRecording::Init()
    {
    char szAppPath[MAX_PATH] = "";
    CString strAppName;

    ::GetModuleFileName(0, szAppPath, MAX_PATH);

    strAppName = szAppPath;
    strAppName = strAppName.Left(strAppName.ReverseFind('\\'));

    m_CustomDir = theApp.CustomSoundDir();
    m_NamesDir = theApp.CustomNamesDir();
    m_DefaultDir.Format("%s\\sounds", strAppName);
    }

CStartListRecording::~CStartListRecording()
    {
    }

float CStartListRecording::MaxDuration()
    {
	if (CStartListRecording::s_MaxDurationHHMM != 0)
		return CStartListRecording::s_MaxDurationHHMM;

	// Find out where files are
	char szAppPath[MAX_PATH] = "";
	CString strAppName, customDir, defaultDir;

	::GetModuleFileName(0, szAppPath, MAX_PATH);

	strAppName = szAppPath;
	strAppName = strAppName.Left(strAppName.ReverseFind('\\'));

	customDir = theApp.CustomSoundDir();
	defaultDir.Format("%s\\sounds", strAppName);

	// Longest times are the on the hour times, so just read through them

	CString name;
	float maximum(0), current;
	for (int i = 0; i < 24; i++)
		{
		name.Format("%s\\cdhour%02d.wav", customDir, i);
		struct _stat buf;
		if (::_stat(name, &buf) == 0) // file exists
			{
			CWave temp(name);
			if (!temp.IsValid()) throw temp.LastError();
			current = temp.getDuration();
			if (current > maximum)
				maximum = current;
			}
		else
			{
			name.Format("%s\\cdhour%02d.wav", defaultDir, i);
			if (::_stat(name, &buf) == 0) // file exists
				{
				CWave temp(name);
				if (!temp.IsValid()) throw temp.LastError();
				current = temp.getDuration();
				if (current > maximum)
					maximum = current;
				}
			else
				{
				throw "Couldn't find the application file " + name;
				}
			}
		}

	return maximum;
    }

CHighTimeSpan CStartListRecording::RealDuration(CHighTime a_Time)
    {
    CString file;
    CHighTimeSpan retVal;
    
    CHighTime spokenTime;

    CHighTimeSpan offset = MM2Span(m_OffsetTime);
    spokenTime = a_Time + offset;
    
    int hr = spokenTime.GetHour();

    if (spokenTime.GetMinute() == 0)
        {
        file.Format("cdhour%02d.wav", spokenTime.GetHour());
        return FloatSecsToSpan(FileDuration(file));
        }
    if (hr > 0 && hr < 10)
        file.Format("cdnum%01d.wav", hr);
    else
        file.Format("cdnum%02d.wav", hr);
    retVal = FloatSecsToSpan(FileDuration(file));
    file.Format("cdnum%02d.wav", spokenTime.GetMinute());
    retVal += FloatSecsToSpan(FileDuration(file));
    retVal += FloatSecsToSpan((float) SHORT_GAP_SIZE);
    return retVal;
    }

float CStartListRecording::FileDuration(CString a_File)
    {
    // Find out where file is
    char szAppPath[MAX_PATH] = "";
    CString strAppName, customDir, defaultDir;

    ::GetModuleFileName(0, szAppPath, MAX_PATH);

    strAppName = szAppPath;
    strAppName = strAppName.Left(strAppName.ReverseFind('\\'));

    customDir = theApp.CustomSoundDir();
    defaultDir.Format("%s\\sounds", strAppName);

    // Get file length
    CString name;
    name.Format("%s\\%s", customDir, a_File);
    struct _stat buf;
    if (::_stat(name, &buf) == 0) // file exists
        {
        CWave temp(name);
		if (temp.IsValid())
			return temp.getDuration();
		else
			{
			AfxMessageBox(temp.LastError(), MB_ICONEXCLAMATION);
			throw temp.LastError();
			}
        }

    name.Format("%s\\%s", defaultDir, a_File);

    if (::_stat(name, &buf) == 0) // file exists
        {
        CWave temp(name);
		if (temp.IsValid())
			return temp.getDuration();
		else
			{
			AfxMessageBox(temp.LastError(), MB_ICONEXCLAMATION);
			throw temp.LastError();
			}
        }
    
    AfxMessageBox("Couldn't find the application file " + name);            
    throw "Couldn't find the application file " + name;
    }

float CStartListRecording::MaxDurationHHMM()
    {

    }

float CStartListRecording::MaxDurationMM()
    {
    if (CStartListRecording::s_MaxDurationMM != 0)
        return CStartListRecording::s_MaxDurationMM;

    // Find out where files are
    char szAppPath[MAX_PATH] = "";
    CString strAppName, customDir, defaultDir;

    ::GetModuleFileName(0, szAppPath, MAX_PATH);

    strAppName = szAppPath;
    strAppName = strAppName.Left(strAppName.ReverseFind('\\'));

    customDir = theApp.CustomSoundDir();
    defaultDir.Format("%s\\sounds", strAppName);

    // Check out each minute
       
    CString name;
    float maximum(0), current;
    for (int i = 1; i < 60; i++)
        {
        name.Format("%s\\cdnum%02d.wav", customDir, i);
        struct _stat buf;
        if (::_stat(name, &buf) == 0) // file exists
            {
            CWave temp(name);
			if (!temp.IsValid()) throw temp.LastError();
            current = temp.getDuration();
            if (current > maximum)
                maximum = current;            
            }
        else
            {
            name.Format("%s\\cdnum%02d.wav", defaultDir, i);
            if (::_stat(name, &buf) == 0) // file exists
                {
                CWave temp(name);
				if (!temp.IsValid()) throw temp.LastError();
                current = temp.getDuration();
                if (current > maximum)
                    maximum = current;            
                }
            else
                {
                throw "Couldn't find the application file " + name;            
                }
            }
        }     

    return maximum;
    }

float CStartListRecording::MaxDurationSS()
    {
    return MaxDurationMM();
    }

bool CStartListRecording::AppendSoundToFile(CHighTime a_Time, CString& a_FileName)
    {
    CString file;
    CHighTimeSpan retVal;
    

    CHighTime spokenTime;
    CHighTimeSpan offset = MM2Span(m_OffsetTime);
        spokenTime = a_Time + offset;


    int hr = spokenTime.GetHour();

    if (spokenTime.GetMinute() == 0)
        {
        file.Format("cdhour%02d.wav", spokenTime.GetHour());
        return CopyAppendWaveFile(FileLocation(file), a_FileName);
        }

    if (hr > 0 && hr < 10)
        file.Format("cdnum%01d.wav", spokenTime.GetHour());
    else
        file.Format("cdnum%02d.wav", spokenTime.GetHour());
    if (CopyAppendWaveFile(FileLocation(file), a_FileName))
        {
        CWave wave(a_FileName);
		if (!wave.IsValid()) throw wave.LastError();
        wave.AddBlank((float)SHORT_GAP_SIZE);
        
        file.Format("cdnum%02d.wav", spokenTime.GetMinute());              
        CWave waveMM(FileLocation(file));
		if (!waveMM.IsValid()) throw waveMM.LastError();
        wave.AddWave(waveMM);
        wave.saveToFile();
        return true;
        }
    else
        return false;
    }


CString CStartListRecording::FileLocation(CString a_File)
    {
    // Find out where files are
    CString name;
    name.Format("%s\\%s", m_CustomDir, a_File);

    struct _stat buf;
    if (::_stat(name, &buf) == 0) // file exists
        {
        return name;
        }
    else
        {
        name.Format("%s\\%s", m_DefaultDir, a_File);
        if (::_stat(name, &buf) == 0) // file exists
            {
            return name;
            }
        else
            {
            AfxMessageBox("Couldn't find the application file " + name);            
            throw;
            }
        }     

    }

int CStartListRecording::ImageIndex() 
	{
	if (IsAfterMsg())
		return 4;
	else
		return 0;
	}