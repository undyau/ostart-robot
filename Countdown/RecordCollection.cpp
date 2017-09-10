// RecordCollection.cpp: implementation of the CRecordCollection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#pragma  warning(disable: 4786)
#include "Countdown.h"
#include "RecordCollection.h"
#include "CustomRecording.h"
#include "TimeRecording.h"
#include "hightime.h"
#include "utils.h"
#include "Wave.h"
#include "ProgressWnd.h"
#include "WaveDisk.h"
#include "WaveReadOnly.h"
#include "BladeMP3EncDll.h"
#include <vector>

extern CCountdownApp theApp;

#define INVALID_SET_FILE_POINTER ((DWORD)-1)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecordCollection::CRecordCollection(): m_TimeRecordsCreated(0), m_Progress(NULL)
    {
	m_Frequency = "1:00";
	m_Version = 2;
    }

CRecordCollection::~CRecordCollection()
    {
    Clear();
    }

bool CRecordCollection::ReloadFromXML(CString a_XMLFileName)
    {
	Clear();
    CStdioFile file;
    if (!file.Open(a_XMLFileName, CFile::modeRead))
        {
        m_LastError = "Unable to open " + a_XMLFileName + " to read from.";
        return false;
        }
    CString theXML;
    if (!file.ReadString(theXML))
        {
        m_LastError = "Unable to open " + a_XMLFileName + " to read from.";
        return false;
        }
    file.Close();

    m_LastError.Empty();
    Clear();
    #define VARBITS(a) #a, a 
	GetXMLValue(theXML, VARBITS(m_Version), 1);
	if (m_Version < 3)
		{
		m_Version = 3; // default to current value in case of save
        m_LastError = a_XMLFileName + " contains data from previous incompatible version of CountDown.";
        return false;
		}
    GetXMLValue(theXML, VARBITS(m_TimeRecordsCreated));
    GetXMLValue(theXML, VARBITS(m_FileName));    
	GetXMLValue(theXML, VARBITS(m_StartTime));    
	GetXMLValue(theXML, VARBITS(m_StopTime));    
	GetXMLValue(theXML, VARBITS(m_Frequency), "1:00"); 
    #undef VARBITS

    GetXMLValue(theXML, "Records", theXML);
    
    int start (-1);
    while ((start = theXML.Find("<Record>", start + 1)) >= 0)
        {
        CString record;
        GetXMLValue(theXML.Mid(start), "Record", record);

        CString type;
        GetXMLValue(record, "Type", type);

        CRecording* rec;
        if (type == "Timed")
            rec = new CTimeRecording(record);
        else
            rec = new CCustomRecording(record);
        if (rec)
            m_Map[rec->Name()] = rec;
		ASSERT(rec);

        }

	t_MapType::iterator iter;
    for(iter = m_Map.begin(); iter != m_Map.end(); iter++)
        {
        TRACE2( "Rec %s follow msg %s\n",(*iter).second->Name(),(*iter).second->FollowMsgName());
        }


    return true;

    }


// Anything that calls this member must delete [] the result

CRecording** CRecordCollection::GetSortedRecords()
    {
    CRecording** array = new CRecording*[m_Map.size()];

    t_MapType::iterator iter;
    int i(0), count(m_Map.size());
    for(iter = m_Map.begin(); iter != m_Map.end(); iter++)
        array[i++] = (*iter).second;


    CRecording* tmp;

    for (i=0; i<count-1; i++) 
         {
         for (int j=0; j<count-1-i; j++) 
             {
             CRecording& a = *array[j+1];
             CRecording& b = *array[j];
             if (IsBefore(a,b)) 
                 {  // compare the two neighbors 
                 tmp = array[j];         // swap array[j] and array[j+1]      
                 array[j] = array[j+1];
                 array[j+1] = tmp;
                 }
             }
        }
    return array;
    }


bool CRecordCollection::IsBefore(CRecording& a_Lhs, CRecording& a_Rhs)
    {
    // Check if the recordings are after one another
    if (Follows(&a_Lhs, &a_Rhs))
        return false;

    if (Follows(&a_Rhs, &a_Lhs))
        return true;

    // Compare start time of ultimate parents
    CRecording* lhs_anchor = &a_Lhs;
    while (lhs_anchor->IsAfterMsg())
		lhs_anchor = m_Map[lhs_anchor->FollowMsgName()];


    CRecording* rhs_anchor = &a_Rhs;
    while (rhs_anchor->IsAfterMsg())
		{
		rhs_anchor = m_Map[rhs_anchor->FollowMsgName()];
		ASSERT(rhs_anchor);
		}

    return lhs_anchor->TimingTime() < rhs_anchor->TimingTime();
    }


bool CRecordCollection::SaveToXML(CString a_XMLFileName, CString a_StartTime, CString a_StopTime)
    {  
    #define VARBITS(a) #a, a

    CString theXML;

// Sort into the order that we want them listed 
    CRecording** vec = GetSortedRecords();

    for (unsigned int i = 0; i < m_Map.size(); i++)
        theXML += ToXML("Record", vec[i]->GetSerialisedXML());

    delete [] vec;

    theXML = ToXML("Records", theXML);
	theXML = ToXML(VARBITS(m_Version)) + theXML;
    theXML = ToXML(VARBITS(m_StartTime)) + theXML;
	theXML = ToXML(VARBITS(m_StopTime)) + theXML;
    theXML = ToXML(VARBITS(m_TimeRecordsCreated)) + theXML;
    theXML = ToXML(VARBITS(m_FileName)) + theXML;
	theXML = ToXML(VARBITS(m_Frequency)) + theXML;
	theXML = ToXML(VARBITS(m_Voice)) + theXML;
	theXML = ToXML("CountDownConfig", theXML);
    

    #undef VARBITS 

    CFile file;
    if (!file.Open(a_XMLFileName, CFile::modeCreate | CFile::modeWrite))
        {
        m_LastError = "Unable to open " + a_XMLFileName + " to write to.";
        return false;
        }
    file.Write(theXML, theXML.GetLength());
    file.Close();
    return true;
    }


 CRecording* CRecordCollection::AddCustomRecord(CString& a_Name, CString& a_FollowMsgName)
     {
    if (RecordExists(a_Name))
        {
        m_LastError = "Record " + a_Name + " already exists in record collection.";
        return NULL;
        }

    CCustomRecording* rec = new CCustomRecording(a_Name, a_FollowMsgName);
    
    if (rec)
        m_Map[a_Name] = rec;
    return rec;

     }

CRecording* CRecordCollection::AddCustomRecord(CString& a_Name, bool a_FinishAtTime, CString a_TimingTime)
    {
    if (RecordExists(a_Name))
        {
        m_LastError = "Record " + a_Name + " already exists in record collection.";
        return NULL;
        }

    if (TimingTimeExists(a_TimingTime))
        {
        m_LastError = "Record with offset " + a_TimingTime + " already exists in record collection.";
        return NULL;
        }

    CCustomRecording* rec = new CCustomRecording(a_Name, a_FinishAtTime, a_TimingTime);
    
    if (rec)
        m_Map[a_Name] = rec;
    return rec;
    }

CRecording* CRecordCollection::AddTimeRecord(bool a_FinishAtTime,  CString a_TimingTime, 
                                             CString a_OffsetTime)
    {
    if (TimingTimeExists(a_TimingTime))
        {
        m_LastError = "Record with offset " + a_TimingTime + " already exists in record collection.";
        return NULL;
        }

    CString name;
    name.Format("Time Message %i", ++m_TimeRecordsCreated);
    CTimeRecording* rec = new CTimeRecording(name, a_FinishAtTime, a_TimingTime, 
		a_OffsetTime);
    if (rec)
        m_Map[name] = rec;
    return rec;
    }

CRecording* CRecordCollection::AddTimeRecord(CString& a_FollowMsgName,
                                             CString a_OffsetTime)
    {
    CString name;
    name.Format("Time Message %i", ++m_TimeRecordsCreated);
    CTimeRecording* rec = new CTimeRecording(name, a_FollowMsgName, a_OffsetTime);

    if (rec)
        m_Map[name] = rec;
    return rec;
    }

CString CRecordCollection::LastError()
    {
    return m_LastError;
    }

void CRecordCollection::Clear()
    {
    t_MapType::iterator iter;
    for(iter = m_Map.begin(); iter != m_Map.end(); iter++)
        {
        delete (*iter).second;
        }
    m_Map.clear();
    }

bool CRecordCollection::RecordExists(CString a_Name)
    {
    t_MapType::iterator iter = m_Map.find(a_Name);
    return iter != m_Map.end();
    }

bool CRecordCollection::TimingTimeExists(CString a_TimingTime)
    {
	for (t_MapType::iterator iter = m_Map.begin(); iter != m_Map.end(); iter++)
        {
        CRecording* rec = (*iter).second;
		if (rec->TimingTime() == a_TimingTime)
            {
    		return true;
            }
        }
	return false;
    }

bool CRecordCollection::HasDependent(CString& a_Name)
	{
	for (t_MapType::iterator iter = m_Map.begin(); iter != m_Map.end(); iter++)
        {
        CRecording* rec = (*iter).second;
		if (rec->IsAfterMsg() && rec->FollowMsgName() == a_Name)
            {
    		return true;
            }
        }
	return false;
	}

bool CRecordCollection::DeleteRecord(CString& a_Name)
	{
	if (!RecordExists(a_Name))
        {
        m_LastError.Format("record %s doesn't exist", a_Name);
		return false;
        }

	if (HasDependent(a_Name))
		{
        m_LastError.Format("record %s has records that run after on it", a_Name);
		return false;
        }


	CRecording* rec = m_Map[a_Name];
	m_Map.erase(a_Name);
	delete rec;

  	int found(0); // count number of Time recordings left
	t_MapType::iterator iter;
    for (iter = m_Map.begin(); iter != m_Map.end(); iter++)
        {
		if (!(*iter).second->IsAfterMsg())
            found++;
        }  
    
    if (found == 0)
        m_TimeRecordsCreated = 0;

	return true;
	}


bool CRecordCollection::MakeRecording(CString a_First, CString& a_Last, CString& a_Every, CString a_FileName)
    {
    // make sure wave file doesn't exist
    if (FileExists(a_FileName))
        {
        DeleteFile(a_FileName);
        }
    if (FileExists(a_FileName))
        {
        AfxMessageBox("File " + a_FileName + " exists and can't be deleted.\nMake sure it isn't opened by another program.");
        return false;
        }
	    
    m_FileName = a_FileName;
    // Rebuild the schedule
    CHighTime First, Last;
	CHighTimeSpan Every;
    First.SetTime(atoi(Word(a_First, 0, ':')),atoi(Word(a_First, 1, ':')),atoi(Word(a_First, 2, ':')));
    Last.SetTime(atoi(Word(a_Last, 0, ':')),atoi(Word(a_Last, 1, ':')),atoi(Word(a_Last, 2, ':')));
	Every.SetHighTimeSpan(0,0,atoi(Word(a_Every, 0, ':')),atoi(Word(a_Every, 1, ':')));
       
    // For each event on list place all occurrences into an ordered list based on start time
    typedef std::map<CHighTime, CRecording*> t_TimeLine;
    t_TimeLine timeLine;

    typedef std::map<CHighTime, CHighTime> t_RealTimes;
    t_RealTimes realTimes;

    t_MapType::iterator iter;

	CHighTime lastStart;
	for(iter = m_Map.begin(); iter != m_Map.end(); iter++)
        {
        CRecording* rec = (*iter).second;
		if (OffsetStartTime(rec,First) == lastStart)
			{
			AfxMessageBox("Recording \"" + rec->Name() + "\" starts at the same time as its predecessor");
            return false;
			}
		lastStart = OffsetStartTime(rec,First);
		}

    SetProgressWndRange(0, m_Map.size());    
    for(iter = m_Map.begin(); iter != m_Map.end(); iter++)
        {
        CRecording* rec = (*iter).second;

        SetProgressWndText("Processing Start Times","Processing start times for recording %s", rec->Name());  
        StepProgressWnd();

        CHighTime progress(First);
        while (progress <= Last)
            {
// ie how long do any preceding snips this rec follows take ?            
            CHighTime t = OffsetStartTime(rec, progress);
            TRACE("Starting %s at %s\n", rec->Name(), t.Format("%H:%M:%S.%s"));
            timeLine[t] = rec;
            realTimes[t] = progress;
            progress = progress + Every;
            }

		if (ProgressWndCancelled()) 
            {
            AfxMessageBox("Create Cancelled");
            return false;
            }
        }


    // Check that no events in the list overlap 
    SetProgressWndRange(0, timeLine.size());
    t_TimeLine::iterator titer;
    CHighTime latestFinish;
    CRecording* latestRec;
    for (titer = timeLine.begin(); titer != timeLine.end(); titer++)
        {
		CHighTime temp = (*titer).first;
		CString tempTime = temp.Format("%H:%M:%S.%s");
        
        SetProgressWndText("Checking for Overlaps","Checking no overlaps for recording at %s", tempTime);   
        StepProgressWnd();

        if (latestFinish.GetStatus() != CHighTime::valid)
            {
            latestFinish = (*titer).first + FloatSecsToSpan((*titer).second->MaxDuration());
            latestRec = (*titer).second;
            }
        else
            {
            if ((*titer).first < latestFinish &&
                !Follows((*titer).second, latestRec))// overlap if we overlap with predecessor OK
                {
                AfxMessageBox("Recording \"" + (*titer).second->Name() + "\" starting at " + 
					(*titer).first.Format("%H:%M:%S.%s") + " overlaps \"" + latestRec->Name() + "\" finishing at " + 
                    latestFinish.Format("%H:%M:%S.%s"));
                return false;
                }
            else
				{
                latestFinish = (*titer).first + FloatSecsToSpan((*titer).second->MaxDuration());
                latestRec = (*titer).second;
				}
            }

        if (ProgressWndCancelled()) 
            {
            AfxMessageBox("Create Cancelled");
            return false;
            }
        }

    
    // Set up first recording and any preceding gap (ie make start on a minute)


    float startGap(0);
	CRecording* prev = NULL;
	CHighTime start;
    titer = timeLine.begin();
    if (titer != timeLine.end())
        {
		CHighTime temp = (*titer).first;

        CHighTime round(temp);
        round.SetDateTime(temp.GetYear(), temp.GetMonth(), temp.GetDay(), temp.GetHour(), temp.GetMinute(), 0);
        CHighTimeSpan gap = temp - round;
        startGap = SpanToFloatSecs(gap);

        CWaveReadOnly gapWave(GapFileName());
        if (startGap  - gapWave.getDuration() >= 0.001) 
            {
            CopyFile(GapFileName(), a_FileName, TRUE);
            
	        CWaveDisk wave(a_FileName); // First file is in there
            wave.AddBlank(startGap - gapWave.getDuration());
            wave.saveToFile();
	        if (!wave.IsValid())
		        {
		        AfxMessageBox(wave.LastError(), MB_ICONEXCLAMATION);
                return false;
		        }

            round = round - CHighTimeSpan(0,0,1,0,0,0);
			AfxMessageBox("Recording will start with a silence to align on minute boundary.\nEffective start of recording is " + round.Format("%H:%M"), MB_ICONINFORMATION);
            }
        else
            startGap = 0;

        (*titer).second->AppendSoundToFile(realTimes[(*titer).first], a_FileName);
        }
    else
        return true; // nothing to do

	CWaveDisk wave(a_FileName); // First file is in there
	if (!wave.IsValid())
		{
		AfxMessageBox("Problem with " + a_FileName + wave.LastError(), MB_ICONEXCLAMATION);
        return false;
		}


	// For each recording on list, add the preceding gap, then add the recording
    SetProgressWndRange(0, timeLine.size() + 1);        
    for (; titer != timeLine.end(); titer++)
        {
        SetProgressWndText("Creating Sounds","Creating sound for time %s", (*titer).first.Format("%H:%M:%S"));  
        StepProgressWnd();

		CHighTime temp = (*titer).first;
		CRecording* rec = (*titer).second;
        TRACE("Recording for %s\n", rec->Name());

		if (prev == NULL)
			{
			start = temp;
			prev = rec;
            // already have first sound in the CWave    
			}
		else  
			{            
			CHighTime end = start + FloatSecsToSpan(wave.getDuration() - startGap);
			if (temp > end)
                {
                CHighTimeSpan gap = temp - end;
                wave.AddBlank(SpanToFloatSecs(gap));
                //wave.saveToFile();
                }    
            
            TCHAR szPath[_MAX_PATH];
            TCHAR szTempName[_MAX_PATH];
            CString tempFile;

            VERIFY(GetTempPath(_MAX_PATH, szPath) != 0);
            VERIFY(GetTempFileName(szPath, _T("afx"), 0, szTempName) != 0);
            tempFile = CString(szTempName) + ".wav";

            rec->AppendSoundToFile(realTimes[temp], tempFile);
            CWave tempWave(tempFile);
			if (!tempWave.IsValid())
				{
				AfxMessageBox(tempWave.LastError(), MB_ICONEXCLAMATION);
				return false;
				}
            if (!wave.AddWave(tempWave))
				{
				DeleteFile(tempFile);
				AfxMessageBox(tempWave.LastError(), MB_ICONEXCLAMATION);
				return false;
				}
            DeleteFile(tempFile);
			}
		

        //COleDateTime endWatch = COleDateTime::GetCurrentTime();
       // COleDateTimeSpan timeTaken = startWatch - endWatch;

        if (ProgressWndCancelled()) 
            {
            AfxMessageBox("Create Cancelled");
            return false;
            }

		prev = rec;
        }
    SetProgressWndRange(0, 100);
    SetProgressWndText("Writing File","Writing file %s - please wait", wave.getFileName());
    wave.saveToFile();
	return true;
    }


bool CRecordCollection::Follows(CRecording* a_Recording, CRecording* a_PossiblePredecessor)
    {
    CRecording* anchor = a_Recording;
    
    while (anchor != a_PossiblePredecessor && anchor && 
           anchor->IsAfterMsg())
        {
        anchor = m_Map[anchor->FollowMsgName()];
        }
    return anchor == a_PossiblePredecessor;
    }

CHighTime CRecordCollection::OffsetStartTime(CRecording* a_Rec, CHighTime a_Time)
    {
    CRecording* anchor = a_Rec;
	CHighTime offsetStartTime(a_Time);
    
    while (true)
        {        
        CString name = anchor->FollowMsgName();
        if (!RecordExists(name) && anchor->IsAfterMsg())
			{
            ASSERT(false);
			return offsetStartTime;
			}

		if (anchor != a_Rec)
			offsetStartTime += anchor->RealDuration(a_Time);

        if (anchor->IsAfterMsg())
            {
			offsetStartTime += HHMMSST2Span(anchor->TimingTime());
			}
		else
			{
			if (anchor->FinishAtTime())
				{
				CHighTimeSpan diff = anchor->RealDuration(a_Time);
				offsetStartTime += HHMMSST2Span(anchor->TimingTime());
				offsetStartTime -= diff;
				}
			else  // start at time
				offsetStartTime += HHMMSST2Span(anchor->TimingTime());

			}
	//	TRACE3("Offset Start Time is %s for %s (time %s)\n", 
	//					offsetStartTime.Format("%H:%M:%S.%s"),anchor->Name(),a_Time.Format("%H:%M:%S.%s"));
        if (anchor->IsAfterMsg())
            anchor = m_Map[name];
        else
            break;
        }
    
	return offsetStartTime;
    }

CRecording* CRecordCollection::GetRec(int a_Index)
    {
    t_MapType::iterator iter;
    int i(0);
    
    for (iter = m_Map.begin(); iter != m_Map.end(); iter++, i++)
        {
        CRecording* rec = (*iter).second;
        if (i == a_Index)
            return rec;
        }
    return NULL;
    }

CRecording* CRecordCollection::GetRec(CString a_Name)
    {
    t_MapType::iterator iter = m_Map.find(a_Name);
    ASSERT(iter != m_Map.end());
	return (*iter).second;
    }

void CRecordCollection::SetProgressWnd(CProgressWnd* a_Progress)
    {
    m_Progress = a_Progress;
    }

void CRecordCollection::ClearProgressWnd()
    {
    m_Progress = NULL;
    }

void CRecordCollection::SetProgressWndRange(int a_Lower, int a_Upper)
    {
    if (m_Progress)
        {
        m_Progress->SetRange(a_Lower, a_Upper);  
        m_Progress->SetPos(a_Lower);
        }
    }

void CRecordCollection::SetProgressWndText(CString a_Caption, LPCTSTR a_Fmt, ...)
    {
    if (m_Progress)
        {
        va_list args;
        TCHAR buffer[512];

        va_start(args, a_Fmt);
        _vstprintf_s(buffer, sizeof(buffer), a_Fmt, args);
        va_end(args);

        m_Progress->SetText(buffer);
		m_Progress->Title(a_Caption);
        }
    }

void CRecordCollection::StepProgressWnd()
    {
    if (m_Progress)
        {
        m_Progress->StepIt();
        m_Progress->PeekAndPump();
        }
    }

bool CRecordCollection::ProgressWndCancelled()
    {
    if (m_Progress)
        return !!m_Progress->Cancelled();
    
    return false;
    }



bool CRecordCollection::ConvertToMp3(CString a_FileName)
    {
    CString waveName;

    SetProgressWndText("Convert to mp3","Converting wave file to mp3"); 

    // make sure mp3 file doesn't exist
    if (FileExists(a_FileName))
        {
        DeleteFile(a_FileName);
        }
    if (FileExists(a_FileName))
        {
        AfxMessageBox("File " + a_FileName + " exists and can't be deleted.\nMake sure it isn't opened by another program.");
        return false;
        }

    HMODULE hLib = LoadLibrary("lame_enc.dll");

    if (!hLib)
        {        
        m_LastError = "Couldn't locate mp3 transformation dll (lame_enc.dll).\nPlease find a copy, ";
        m_LastError += "\nand copy to same folder as this program\nNote: some versions work better than others.";
        AfxMessageBox(m_LastError);
        return false;
        }

 
// Find required functions in dll
    

    BEINITSTREAM beInitStream;
    BEENCODECHUNK beEncodeChunk;
    BEDEINITSTREAM beDeinitStream;
    BECLOSESTREAM beCloseStream;
    BEWRITEVBRHEADER beWriteVBRHeader;
    BEWRITEINFOTAG beWriteInfoTag;

    beInitStream = (BEINITSTREAM)GetProcAddress(hLib, TEXT_BEINITSTREAM);
    beEncodeChunk = (BEENCODECHUNK)GetProcAddress(hLib, TEXT_BEENCODECHUNK);
    beDeinitStream = (BEDEINITSTREAM)GetProcAddress(hLib, TEXT_BEDEINITSTREAM);
    beCloseStream = (BECLOSESTREAM)GetProcAddress(hLib, TEXT_BECLOSESTREAM);
    beWriteVBRHeader = (BEWRITEVBRHEADER)GetProcAddress(hLib, TEXT_BEWRITEVBRHEADER);
    beWriteInfoTag = (BEWRITEINFOTAG)GetProcAddress(hLib, TEXT_BEWRITEINFOTAG);

    if(!beInitStream || !beEncodeChunk || !beDeinitStream || !beDeinitStream ||
       !beCloseStream || !beWriteVBRHeader)
        {
        m_LastError = "Unable to load function(s) from lame_enc.dll.";
        FreeLibrary((HMODULE)hLib);
        return false;
        }

// Load wave file header

    HANDLE hOutputFile(NULL);
    HANDLE hInputFile(NULL);
    DWORD streamhandle(0);
    BYTE* outBuf = NULL;
    PSHORT inBuf = NULL;
    try 
        {
        waveName = a_FileName;
        if (a_FileName.Right(4).CompareNoCase(".mp3"))
            throw "File must have mp3 suffix";
        waveName = waveName.Left(waveName.GetLength() - 3) + "wav";
        CWaveReadOnly wave(waveName); 
     
 // Initialise transformation, get buffer  
        
        BE_CONFIG config;
        
        memset(&config,0,sizeof(config));					// clear all fields
	
// use the LAME config structure
	    config.dwConfig = BE_CONFIG_LAME;

// settings for testcase.wav
	    config.format.LHV1.dwStructVersion	= 1;
	    config.format.LHV1.dwStructSize		= sizeof(config);		
	    config.format.LHV1.dwSampleRate		= 22050;				// INPUT FREQUENCY
	    config.format.LHV1.dwReSampleRate		= 0;					// DON"T RESAMPLE
	    config.format.LHV1.nMode				= BE_MP3_MODE_MONO;	// OUTPUT IN STREO
	    config.format.LHV1.dwBitrate			= 128;					// MINIMUM BIT RATE
	    config.format.LHV1.nPreset			= LQP_VOICE_QUALITY;		// QUALITY PRESET SETTING
	    config.format.LHV1.dwMpegVersion		= MPEG2;				// MPEG VERSION (I or II)
	    config.format.LHV1.dwPsyModel			= 0;					// USE DEFAULT PSYCHOACOUSTIC MODEL 
	    config.format.LHV1.dwEmphasis			= 0;					// NO EMPHASIS TURNED ON
	    config.format.LHV1.bOriginal			= TRUE;					// SET ORIGINAL FLAG
	    config.format.LHV1.bWriteVBRHeader	= TRUE;					// Write INFO tag
        config.format.LHV1.nVBRQuality = 3; 

        DWORD chunksize;     // size of input buffer in shorts
        DWORD outBufSize;    // byte size of buffer that will be required
        

        if (beInitStream(&config, &chunksize, &outBufSize, &streamhandle) != BE_ERR_SUCCESSFUL)
            throw "Error on call to beInitStream.";

        
        outBuf = new BYTE[outBufSize];        
        DWORD InSampleSize = wave.getSampleSize();
        inBuf = new SHORT[chunksize];
        DWORD bytesWritten;
        DWORD dataRead(0);
        DWORD written;

        
// Open input wave files        
        DWORD position = wave.getDataStartPosition();
        HANDLE hInputFile = CreateFile(wave.getFileName(), GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_READONLY | FILE_FLAG_SEQUENTIAL_SCAN,NULL);
        if (hInputFile==INVALID_HANDLE_VALUE)
            throw CString("Unable to open " + wave.getFileName());
        
        if (SetFilePointer(hInputFile, position, 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
            throw CString("Unable to position within file " + wave.getFileName());
        

// Open output mp3 file
        HANDLE hOutputFile = CreateFile(a_FileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
        if (hInputFile==INVALID_HANDLE_VALUE)
            throw CString("Unable to open " + a_FileName);

        DWORD waveFileSize = wave.getDataLength();
        DWORD stepSize = waveFileSize/100;
        SetProgressWndRange(0, 100);        
        DWORD bytesRead, stepRead;

// Read the wave file N samples at a time and encode this chunk, writing to mp3 file
        while (dataRead < waveFileSize)    
            { 
            if (stepRead > stepSize)
                {
                StepProgressWnd();
                stepRead = 0;
                if (ProgressWndCancelled()) 
                    throw CString("Create Cancelled");
                }
            
            if (!ReadFile(hInputFile, (void*)inBuf, chunksize*sizeof(SHORT), &bytesRead, NULL))
                throw CString("unable to read " + wave.getFileName());
                
            dataRead += bytesRead;
            stepRead += bytesRead;
    
            if (beEncodeChunk(streamhandle, bytesRead/InSampleSize, inBuf, outBuf, &bytesWritten) != BE_ERR_SUCCESSFUL)
                throw CString("Unable to encode data - call to beEnCodeChunk failed");

            if (!WriteFile(hOutputFile, outBuf, bytesWritten, &written, NULL) || written != bytesWritten)
                throw CString("Unable to write bytes to output file " + a_FileName);

            //TRACE("Read %d (%d samples), translated, got %d back and wrote them\n", bytesRead, samples, bytesWritten);
            }

// Clean up      

        if (beDeinitStream(streamhandle, outBuf, &bytesWritten) != BE_ERR_SUCCESSFUL)
            throw CString("Call to beDeinitStream failed");
        if (bytesWritten > 0 && !WriteFile(hOutputFile, outBuf, bytesWritten, &written, NULL) || written != bytesWritten)
            throw CString("Unable to write bytes to output file " + a_FileName);

        beCloseStream(streamhandle);
        CloseHandle(hInputFile);
        CloseHandle(hOutputFile);
        
        if (beWriteInfoTag)
            beWriteInfoTag(streamhandle, a_FileName);
        else
            beWriteVBRHeader(a_FileName);

        FreeLibrary((HMODULE)hLib);  

        delete [] outBuf;
        delete [] inBuf;

        DeleteFile(waveName);
        return true;
        }

    catch (CString err)
        {
        m_LastError = err;
        if (streamhandle) beCloseStream(streamhandle);
        CloseHandle(hInputFile);
        CloseHandle(hOutputFile);
        FreeLibrary((HMODULE)hLib);
        delete [] outBuf;
        delete [] inBuf;
        throw err;
        }
    catch (...)
        {
        CloseHandle(hInputFile);
        CloseHandle(hOutputFile);
        FreeLibrary((HMODULE)hLib);
        delete [] outBuf;
        delete [] inBuf;
        return false;
        }

    }

CString CRecordCollection::GapFileName()
    {
    char szAppPath[MAX_PATH] = "";
    CString strAppName, result;

    ::GetModuleFileName(0, szAppPath, MAX_PATH);

    strAppName = szAppPath;
    strAppName = strAppName.Left(strAppName.ReverseFind('\\'));

    result.Format("%s\\sounds\\gap.wav", strAppName);
    return result;
    }

bool CRecordCollection::ModifyRecord(CString& a_Name,
									 bool a_IsCustom,
									 bool a_IsFollow,
									 CString& a_TimingTime,
									 bool a_FinishAtTime,
									 CString& a_FollowMsgName,
									 CString& a_OffsetTime)
	{
	CRecording* rec	= GetRec(a_Name);
	
	rec->IsAfterMsg(a_IsFollow);
	if (!a_IsFollow)
		{
		rec->TimingTime(a_TimingTime);
		rec->FinishAtTime(a_FinishAtTime);
		}
	else
		rec->FollowMsgName(a_FollowMsgName);

	if (!a_IsCustom)
		{
		CTimeRecording* trec = static_cast<CTimeRecording*>(rec);
		trec->OffsetTime(a_OffsetTime);
		}

	return true;
	}

								 