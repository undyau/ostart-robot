#include "stdafx.h"
#include "Utils.h"
#include "HighTime.h"
#include <sys/stat.h>
#include "Wave.h"
#include <ATLComTime.h >
#include <time.h>

CHighTimeSpan FloatSecsToSpan(float a_Secs)
    {
    CHighTimeSpan retVal;

    // days
    int days = (int)(a_Secs/(24*60*60));
    a_Secs -= days*24*60*60;

    // hours
    int hours = (int)(a_Secs/(60*60));
    a_Secs -= hours*60*60;

    // mins
    int mins = (int)(a_Secs/60);
    a_Secs -= mins*60;

    // secs
    int secs = (int)a_Secs;
    a_Secs -= secs;

    int millis = (int)(1000*a_Secs);

    retVal.SetHighTimeSpan(days, hours, mins, secs, millis);
    return retVal;
    }

float SpanToFloatSecs(CHighTimeSpan& a_Span)
	{
	float result = (float)atof(a_Span.Format("%S.%s"));
	result += a_Span.GetMinutes() * 60;
	result += a_Span.GetHours() * 60 * 60;
	result += a_Span.GetDays() * 60 * 60 * 24;
    return result;
	}

CString HighTimeAsStr(CHighTime const & a_Time)
	{
	CString s;
	s.Format("%2.2d/%2.2d/%4.4d %s", a_Time.GetDay(), a_Time.GetMonth(), a_Time.GetYear(), a_Time.Format("%H:%M:%S.%s"));
	return s;
	}

CHighTimeSpan HHMMSST2Span(CString a_Time)
    {
    int hours, mins, secs, milli(0);
	int wc(1);
	for (int i = 0; i < a_Time.GetLength();i++)
		if (a_Time[i] == ':')
			wc++;
	if (wc < 3) a_Time = "00:" + a_Time;
	if (wc < 2) a_Time = "00:" + a_Time;

    hours = atoi(Word(a_Time, 0, ':'));
    mins = atoi(Word(a_Time, 1, ':'));
    secs = atoi(Word(Word(a_Time, 2, ':'),0));
    if (a_Time.Find(".", 0) > 0)
        {
        milli = (int)(1000 * atof("1." + Word(a_Time, 1, '.')));
        milli -= 1000;
        }
    CHighTimeSpan retVal;
    retVal.SetHighTimeSpan(0,hours, mins, secs, milli);
    return retVal;
    }

CHighTimeSpan MM2Span(CString a_Time)
    {
    int mins = atoi(a_Time);
    CHighTimeSpan retVal;
    retVal.SetHighTimeSpan(0,0, mins, 0, 0);
    return retVal;
    }

bool ToCHighTime(CString a_XmlTime, CHighTime& a_OutputTime)
	{
	// 2017-09-24T10:26:00.000     (OE from Ron)
	// 2012-12-26T14:36:07Z        (IOF example)
	// 2012-12-26T15:36:07+01:00   (IOF example)
	// 2012-12-26T08:36:07.9-06:00 (IOF example)
	// 2017-09-16T10:45:00+10:00   (MEOS example)
	// 2017-09-16T01:30:00Z        (MEOS using UTC)

/*	CHighTime(const LARGE_INTEGER dateSrc);
	CHighTime(const LONGLONG dateSrc);
#if defined(USE_MFC)
	CHighTime(const COleDateTime &dateSrc);
	CHighTime(const CTime &dateSrc);
#endif
	CHighTime(const SYSTEMTIME &systimeSrc);
	CHighTime(const FILETIME &filetimeSrc);
	//	CHighTime(const time_t timeSrc);
	CHighTime(int nYear, int nMonth, int nDay,
		int nHour, int nMinute, int nSecond,
		int nMilli = 0, int nMicro = 0, int nNano = 0);

	tm outtime;
	strptime(a_Time, "", &tm,);*/
	int nYear, nMonth, nDay, nHour, nMinute, nSecond;

	if (a_XmlTime.Right(1) != "Z")
		{
	/*	nYear = atoi(a_XmlTime.Left(4));
		nMonth = atoi(a_XmlTime.Mid(5, 2));
		nDay = atoi(a_XmlTime.Mid(8, 2));*/
		a_OutputTime = CHighTime();
		nHour = atoi(a_XmlTime.Mid(11, 2));
		nMinute = atoi(a_XmlTime.Mid(14, 2));
		nSecond = atoi(a_XmlTime.Mid(17, 2));
		a_OutputTime.SetTime(nHour, nMinute, nSecond);
		return true;
		}
	else
		{
		AfxMessageBox("Couldn't parse time " + a_XmlTime + " start times in UTC are not yet supported", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}
	}

CString Word(const CString a_Text, const unsigned int a_Index, const char a_Delim, const CString a_DefaultValue)
    {
    unsigned int i;
    int delim_pos;
    int next_delim_pos;

    i = 0;

    delim_pos = -1;
    while(1)
        {
        next_delim_pos = a_Text.Find(a_Delim, delim_pos + 1);
        if (next_delim_pos < 0)
            {
            if (i == a_Index)
                return a_Text.Mid(delim_pos + 1);
            return a_DefaultValue;
            }
        if (i == a_Index)
            {
            if (a_Delim == ' ')
                {
                CString s = a_Text.Mid(delim_pos + 1, next_delim_pos - delim_pos - 1);
                s.TrimLeft();
                s.TrimRight();
                return s;
                }
            return a_Text.Mid(delim_pos + 1, next_delim_pos - delim_pos - 1);
            }
        delim_pos = next_delim_pos;
        if (a_Delim == ' ')
            {
            // step over all the spaces, treating them as a single delimiter
			int max = a_Text.GetLength();
            while (delim_pos < max && a_Text[delim_pos] == ' ')
                ++delim_pos;
			if (delim_pos == max)
				return a_DefaultValue;
            --delim_pos;
            }
        ++i;
        }
    }

int WordPosition(CString& a_Text, int a_Index, char a_Delim)
    {
    for (int i = 0, ps = 0, pf = 0; ps < a_Text.GetLength();)
        {
        while (a_Text[ps] == a_Delim)
            ps++;
        pf = ps;

        while (a_Text[pf] != a_Delim && pf < a_Text.GetLength())
            pf++;

        if (pf == a_Text.GetLength())
            pf = a_Text.GetLength() - 1;

        if (pf == (a_Text.GetLength() - 1) || a_Text[pf] == a_Delim)
            {
            if (i == a_Index)
                return ps;
            else
                {
                i++;
                ps = pf;
                }
            }
        }
    return -1;
    }

bool FileExists(CString a_File)
    {
    struct stat buf;
    return !stat(a_File, &buf);
    }

bool CopyAppendWaveFile(CString a_Source, CString a_Target)
    {
    if (FileExists(a_Target))
        {
        CWave twave(a_Target);
		if (!twave.IsValid()) throw twave.LastError();
        CWave swave(a_Source);
		if (!swave.IsValid()) throw swave.LastError();
        bool retVal =  twave.AddWave(swave);
        twave.saveToFile();
        return retVal;
        }
    else
        return !!CopyFile(a_Source, a_Target, TRUE);
    }

CString ToXML(CString a_Label, char* a_Value)
    {
    CString value(a_Value);
    return ToXML(a_Label, value);
    }

CString ToXML(CString a_Label, CString& a_Value)
    {
    CString result;
    result.Format("<%s>%s</%s>", a_Label, a_Value, a_Label);
    return result;
    }

CString ToXML(CString a_Label, bool a_Value)
    {
    CString result;
    result.Format("<%s>%i</%s>", a_Label, a_Value, a_Label);
    return result;
    }

CString ToXML(CString a_Label, int a_Value)
    {
    CString result;
    result.Format("<%s>%i</%s>", a_Label, a_Value, a_Label);
    return result;
    }

void GetXMLValue(CString& a_SomeXML, CString a_Label, CString& a_Variable, CString a_Default)
    {
    int p1, p2;
    CString startLabel, endLabel;
    startLabel.Format("<%s>", a_Label);
    endLabel.Format("</%s>", a_Label);
    
    p1 = a_SomeXML.Find(startLabel);
    p2 = a_SomeXML.Find(endLabel);

    if (p1 < 0 || p2 < p1)
        {
        a_Variable = a_Default;
        return;
        }
    
    p1 += startLabel.GetLength();
    a_Variable = a_SomeXML.Mid(p1, p2 -p1);
    }

void GetXMLValue(CString& a_SomeXML, CString a_Label, bool& a_Variable, bool a_Default)
    {
    CString boolVal;
    GetXMLValue(a_SomeXML, a_Label, boolVal);
    if (boolVal.IsEmpty())
        a_Variable = a_Default;
    else
        a_Variable = !!atoi(boolVal);
    }

void GetXMLValue(CString& a_SomeXML, CString a_Label, int& a_Variable, int a_Default)
    {
    CString intVal;
    GetXMLValue(a_SomeXML, a_Label, intVal);
    if (intVal.IsEmpty())
        a_Variable = a_Default;
    else
        a_Variable = atoi(intVal);
    }

bool ValidPCFileName(const CString &a_Value)
    {
    if (a_Value.IsEmpty())
        {
        return false;
        }
	if (a_Value.GetLength() > MAX_PATH)
		{
		return false;
		}
    if (a_Value.FindOneOf("*?<>|") >= 0)
        {
        return false;
        }
    return true;
    }

void AFXAPI DDV_PCFileName(CDataExchange* pDX, int nIDC, const CString &a_Value)
    {
    if (pDX->m_bSaveAndValidate)
        {
        if (!ValidPCFileName(a_Value))
            {
            pDX->PrepareEditCtrl(nIDC);
            CString message;
            message.Format("'%s' is not a valid PC file name", a_Value);
            AfxMessageBox(message, MB_ICONINFORMATION);
            pDX->Fail();
            }
        }
    }

