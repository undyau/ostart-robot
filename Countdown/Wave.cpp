 // Wave.cpp: implementation of the CWave class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Wave.h"
#include <queue>

const int c_sample_rate = 22050;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWave::CWave(CString _fileName):  m_Valid(true)
    {
    extraParam = NULL;
    wave=NULL;
    fileName=_fileName.operator LPCTSTR();
    extraParamLength = 0;
    fact.samplesNumber=-1;
    HANDLE hF = CreateFile(fileName.data(),GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_READONLY,NULL);
    if (hF==INVALID_HANDLE_VALUE)
        {
        LPVOID lpMsgBuf;
        FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                GetLastError(),
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                (LPTSTR) &lpMsgBuf,
                0,
                NULL );
        m_LastError = (LPTSTR) lpMsgBuf;
        m_Valid = false;
        return;
        }
    DWORD read;
    ReadFile(hF,(LPVOID)&riff,RIFF_SIZE,&read,NULL);
    ReadFile(hF,(LPVOID)&fmt, FMT_SIZE - 2, &read,NULL);

    if (fmt.wFormatTag!=1)
        {
        CloseHandle(hF);
        m_LastError = "Unable to handle this format - can only handle format 1 (PCM) files.";
        m_Valid = false;
        return;
        }
    if (fmt.fmtSIZE != FMT_SIZE -8 && fmt.fmtSIZE != FMT_SIZE -2 -8)
        {
        CloseHandle(hF);
        m_LastError = "Unable to handle this format - fmt chunk is unexpected length.";
        m_Valid = false;
        return;
        }
    if (fmt.wBitsPerSample != 16)
        {
        CloseHandle(hF);
        m_LastError = "Unable to handle this format - only 16 bits/sample supported.";
        m_Valid = false;
        return;
        }
    if (fmt.nSamplesPerSec != 22050)
        {
        m_LastError = "Unable to handle this format - only 22050 Hz supported.";
        CloseHandle(hF);
        m_Valid = false;
        return;
        }
    if (fmt.nAvgBytesPerSec != 44100)
        {
        CloseHandle(hF);
        m_LastError = "Unable to handle this format - only 44100 Bytes/sec supported.";
        m_Valid = false;
        return;
        }
    if (fmt.nChannels != 1)
        {
        CloseHandle(hF);
        m_LastError = "Unable to handle this format - only single channel (mono) is supported.";
        m_Valid = false;
        return;
        }
	if (fmt.fmtSIZE == FMT_SIZE -8)  
		{
		char buf[10];
		// Need to read (and throw away) the wExtraBits field
		ReadFile(hF,(LPVOID)&buf, 2, &read,NULL);
		}

    bool needFactFixup(false);
    ReadFile(hF,(LPVOID)&fact.factID,4,&read,NULL);

    if (strncmp(fact.factID, "fact", 4))
        {
        if (!strncmp(fact.factID, "data", 4))
            {
			memcpy_s(data.dataID, sizeof(data.dataID), fact.factID, 4);
			memcpy_s(fact.factID, sizeof(fact.factID), "fact", 4);
            fact.factSIZE = FACT_SIZE - 8;
            needFactFixup = true;
            }
        else
            {
            CloseHandle(hF);
            m_LastError = "Unable to handle this format - unexpected chunk found when expecting fact or data.";
            m_Valid = false;
            return;
            }
        }
    else
        {
        ReadFile(hF,(LPVOID)&fact.factSIZE,FACT_SIZE-4,&read,NULL);
        }

    if (fact.factSIZE != FACT_SIZE - 8)
        {
        CloseHandle(hF);
        m_LastError = "Unable to handle this format - fact chunk is unexpected length.";
        m_Valid = false;
        return;
        }


    if (needFactFixup) //  found data chunk when looking for fact chunk
        {
        ReadFile(hF,(LPVOID)&data.dataSIZE,DATA_SIZE-4,&read,NULL);
        fact.samplesNumber = data.dataSIZE / (fmt.wBitsPerSample/8);  // nb, should be 2 byte per sample, given other restrictions
        }
    else
        {
        ReadFile(hF,(LPVOID)&data,DATA_SIZE,&read,NULL);
        }

    if (fmt.nSamplesPerSec != (c_sample_rate)) //  /fmt.wBitsPerSample))
        {
        CloseHandle(hF);
        m_LastError.Format("Unable to use this file, sampling rate in file is %i, 22050Hz is required.", fmt.nSamplesPerSec);
        m_Valid = false;
        return;
        }

    wave = new BYTE[data.dataSIZE];
    bufSize = data.dataSIZE;
    ReadFile(hF,(LPVOID)wave,data.dataSIZE,&read,NULL);    

    CloseHandle(hF);
}

float CWave::getDuration()
    {
    if (m_Valid) return ((float)fact.samplesNumber)/(float)(fmt.nSamplesPerSec);
    throw m_LastError;
    }

void CWave::AddBlank(float a_Duration)
    {
    if (!m_Valid) throw m_LastError;
    DWORD blank_len = DWORD(a_Duration * (float) c_sample_rate * (float) fmt.nChannels * (float) fmt.nBlockAlign);
    if (blank_len & 1)
        ++blank_len;

    DWORD sample_count = (DWORD)(a_Duration * (float) c_sample_rate);

    data.dataSIZE += blank_len;
    fact.samplesNumber += sample_count;

    if (data.dataSIZE > bufSize)  // else already have that much space, zero filled
        {
        BYTE* newWave = new BYTE[data.dataSIZE];
        BYTE* oldWave = wave;
        memset(newWave, 0, data.dataSIZE);
        memcpy(newWave, oldWave, data.dataSIZE - blank_len);
        wave = newWave;
        delete[] oldWave;
        bufSize = data.dataSIZE;
        }
    }

bool CWave::AddWave(const CWave& w)
    {
    if (!m_Valid) throw m_LastError;

    if (fmt.wFormatTag != w.fmt.wFormatTag ||
        fmt.nChannels  != w.fmt.nChannels ||
        fmt.nSamplesPerSec != w.fmt.nSamplesPerSec ||
        fmt.nAvgBytesPerSec != w.fmt.nAvgBytesPerSec ||
        fmt.nBlockAlign != w.fmt.nBlockAlign ||
        fmt.wBitsPerSample != w.fmt.wBitsPerSample)
        {
        AfxMessageBox("Unable to copy files, they are not in identical format.");
        return false;
        }


    if (data.dataSIZE + w.data.dataSIZE > bufSize)
        {
        BYTE* newWave = new BYTE[data.dataSIZE + w.data.dataSIZE];
        memcpy(newWave, wave, data.dataSIZE);
        memcpy(newWave + data.dataSIZE, w.wave, w.data.dataSIZE);

        data.dataSIZE += w.data.dataSIZE;
        fact.samplesNumber += w.fact.samplesNumber;    
        delete[] wave;
        wave = newWave;
        bufSize = data.dataSIZE;
        }
    else
        {
        memcpy(wave + data.dataSIZE, w.wave, w.data.dataSIZE);
        data.dataSIZE += w.data.dataSIZE;
        fact.samplesNumber += w.fact.samplesNumber;    
        }

    return true;
    }

CWave::CWave(){
    extraParam = NULL;
    wave=NULL;
    fileName="";
    fact.samplesNumber=-1;
    m_Valid = true;
}
CWave::CWave(const CWave& w){
    init(w);
}
CWave& CWave::operator= (const CWave &w){
    init(w);
    return *this;
}
CWave::~CWave(){
    if (wave)
        delete[] wave;
}


void CWave::init(const CWave& w){
    fmt = w.fmt;
    riff = w.riff;
    data = w.data;
    fact = w.fact;
    fileName = w.fileName;

    extraParamLength = w.extraParamLength;
    extraParam = w.extraParam;
    wave = new BYTE[data.dataSIZE];
    memcpy(wave,w.wave,data.dataSIZE);
    bufSize = data.dataSIZE;
    m_Valid = w.m_Valid;
    m_LastError = w.m_LastError;
}


void CWave::saveToFile()
    {
    if (!m_Valid) throw m_LastError;

    HANDLE hFile = CreateFile(fileName.data(),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    DWORD written;
    WriteFile(hFile,(LPCVOID)&riff,RIFF_SIZE,&written,NULL);
    WriteFile(hFile,(LPCVOID)&fmt,FMT_SIZE,&written,NULL);
    if (fmt.wFormatTag>1){
        WriteFile(hFile,(LPCVOID)&extraParamLength,2,&written,NULL);
        if (extraParamLength>0)
            WriteFile(hFile,(LPCVOID)extraParam,extraParamLength,&written,NULL);
    }
    if (fact.samplesNumber>-1){
        WriteFile(hFile,(LPCVOID)"fact",4,&written,NULL);
        WriteFile(hFile,(LPCVOID)&fact,FACT_SIZE,&written,NULL);
    }
    WriteFile(hFile,(LPCVOID)&data,DATA_SIZE,&written,NULL);
    WriteFile(hFile,(LPCVOID)wave,data.dataSIZE,&written,NULL);

    CloseHandle(hFile);
}

CString CWave::getFileName()
    {
    if (!m_Valid) throw m_LastError;
    CString s = fileName.c_str();
    return s;
    }

void CWave::setFileName(string _fileName)
    {
    if (!m_Valid) throw m_LastError;
    fileName=_fileName;
    }

string CWave::getFileFolder(string fullPath)
    {
    string::size_type last= fullPath.find_last_of("\\");
    if (last==string::npos)
        return "";
    char* buffer = new char[fullPath.size()];
    memset(buffer,0,fullPath.size());
#pragma warning(disable:4996)
    string::size_type copied = fullPath.copy(buffer,last+1);
#pragma warning(default:4996)
    string folder(buffer);
    delete[] buffer;
    return folder;
    }

string CWave::getFileTitle(string fullPath)
    {
    int begin= fullPath.find_last_of("\\");
    if (begin ==string::npos)
        begin=-1;
    string::size_type end= fullPath.find_last_of(".");

    char* buffer = new char[end-begin];
    memset(buffer,0,end-begin);
#pragma warning(disable:4996)
    fullPath.copy(buffer,end-begin-1,begin+1);
#pragma warning(default:4996)
    string title(buffer);
    delete[] buffer;
    return title;
    }


void CWave::setExpectedMaxDuration(float a_Duration)
    {
    if (!m_Valid) throw m_LastError;
    DWORD size;
    size = DWORD(a_Duration * (float) c_sample_rate * (float) fmt.nChannels * (float) fmt.nBlockAlign);
    if (size & 1)
        ++size;

    if (size > bufSize)
        {
        BYTE* newWave = new BYTE[size];
        memset(newWave, 0, size);
        memcpy(newWave, wave, data.dataSIZE);
        bufSize = size;
        delete [] wave;
        wave = newWave;
        }
    }

void CWave::getAmplitude(short& a_Peak, short& a_Avg)
    {
    if (!m_Valid) throw m_LastError;

    DWORD i;
    short* buf = (short*)wave;
    long total(0);
    long maxTotal(0);
    std::queue<short> samples;
    const int c_StatCount = 400;
    
    a_Peak = 0;
    a_Avg = 0;

    if (fact.samplesNumber < c_StatCount)   // sample too small to smooth over
        return;

    for (i = 0; i < c_StatCount; i++)
        {
        short val = ::abs(*(buf+i));
        samples.push(val);
        total += ::abs(val);
        if (val > a_Peak)
            a_Peak = val;
        }

    maxTotal = total;

    for (; i < fact.samplesNumber; i++)
        {
        short val = ::abs(*(buf+i));
        total -= samples.front();
        samples.pop();
        samples.push(val);
        total += val;

        if (total > maxTotal)
            maxTotal = total;        

        if (val > a_Peak)
            a_Peak = val;

        }

    //TRACE2("Peak val was %d, rolling avg was %d\n",  peak, (maxTotal/50));
    a_Avg = (short)(maxTotal/c_StatCount);
    }

void CWave::Normalise(float a_BoostFactor)
    {
    if (!m_Valid) throw m_LastError;

    ASSERT(a_BoostFactor < 2 && a_BoostFactor > 0.01);
    
    short* buf = (short*)wave;

    for (DWORD i = 0; i < fact.samplesNumber; i++)
        {
        short val = *(buf+i);
        float fval = (float)val;
        fval *= a_BoostFactor;
        if (fval > 32767)
            fval = 32767;
        else if (fval < -32768)
            fval = -32768;

        *(buf+i) = (short) fval;
        }

    }