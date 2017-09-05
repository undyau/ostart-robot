 // Wave.cpp: implementation of the CWaveDisk class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaveDisk.h"

const int c_sample_rate = 22050;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWaveDisk::CWaveDisk(CString _fileName) : m_Valid(false)
    {
    factChunkFound = false;
    extraParam = NULL;
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
        LocalFree( lpMsgBuf );
        m_Valid = false;
		return;
        }
    DWORD read;
    ReadFile(hF,(LPVOID)&riff,RIFF_SIZE,&read,NULL);
    ReadFile(hF,(LPVOID)&fmt,FMT_SIZE,&read,NULL);
    if (fmt.wFormatTag!=1)
        {
        CloseHandle(hF);
        m_LastError = "Unable to handle this format - can only handle format 1 (PCM) files.";
        return;
        }
    if (fmt.fmtSIZE != FMT_SIZE -8 && fmt.fmtSIZE != FMT_SIZE -2 -8)
        {
        m_LastError = "Unable to handle this format - fmt chunk is unexpected length.";
        return;
        }
    if (fmt.wBitsPerSample != 16)
        {
        CloseHandle(hF);
        m_LastError = "Unable to handle this format - only 16 bits/sample supported.";
        return;
        }
    if (fmt.nChannels != 1)
        {
        CloseHandle(hF);
        m_LastError = "Unable to handle this format - only single channel (mono) is supported.";
        return;
        }


    ReadFile(hF,(LPVOID)&fact.factID,4,&read,NULL);

    if (strncmp(fact.factID, "fact", 4))
        {
        factChunkFound = false;
        if (!strncmp(fact.factID, "data", 4))
            {
            memcpy_s(data.dataID, sizeof(data.dataID), fact.factID, 4);
            memcpy_s(fact.factID, sizeof(fact.factID), "fact", 4);
            fact.factSIZE = FACT_SIZE - 8;
            }
        else
            {
            CloseHandle(hF);
            m_LastError = "Unable to handle this format - unexpected chunk found when expecting fact or data.";
            return;
            }
        }
    else
        {
        factChunkFound = true;
        ReadFile(hF,(LPVOID)&fact.factSIZE,FACT_SIZE-4,&read,NULL);
        }

    if (fact.factSIZE != FACT_SIZE - 8)
        {
        CloseHandle(hF);
        m_LastError = "Unable to handle this format - fact chunk is unexpected length.";
        return;
        }


    if (!factChunkFound) //  found data chunk when looking for fact chunk
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
        CString temp;
        temp.Format("Unable to use this file, sampling rate in file is %i, 22050Hz is required.", fmt.nSamplesPerSec);
        m_LastError = temp;
        return;
        }

	m_Valid = true;
    CloseHandle(hF);
    }


CWaveDisk::~CWaveDisk()
    {
    }


void CWaveDisk::saveToFile()
    {
	if (!m_Valid)
		throw m_LastError;

    HANDLE hFile = CreateFile(fileName.data(),GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

    DWORD written;
    WriteFile(hFile,(LPCVOID)&riff,RIFF_SIZE,&written,NULL);
    WriteFile(hFile,(LPCVOID)&fmt,FMT_SIZE,&written,NULL);
    if (fmt.wFormatTag>1)
        {
        WriteFile(hFile,(LPCVOID)&extraParamLength,2,&written,NULL);
        if (extraParamLength>0)
            WriteFile(hFile,(LPCVOID)extraParam,extraParamLength,&written,NULL);
        }
    if (factChunkFound)
        {
        if (fact.samplesNumber>-1)
            {
            WriteFile(hFile,(LPCVOID)"fact",4,&written,NULL);
            WriteFile(hFile,(LPCVOID)&fact,FACT_SIZE,&written,NULL);
            }
        }
    WriteFile(hFile,(LPCVOID)&data,DATA_SIZE,&written,NULL);
    
    // No need to write data, it should already be there

    CloseHandle(hFile);
    }


void CWaveDisk::AddBlank(float a_Duration)
    {
	if (!m_Valid)
		throw m_LastError;
    DWORD blank_len = DWORD(a_Duration * (float) c_sample_rate * (float) fmt.nChannels * (float) fmt.nBlockAlign);
    if (blank_len & 1)
        ++blank_len;

    DWORD sample_count = (DWORD)(a_Duration * (float) c_sample_rate);

    BYTE* blanks = new BYTE[blank_len];
    memset(blanks, 0, blank_len);  
    
    // Open the wave file
    HANDLE hF = CreateFile(fileName.data(),GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
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
        LocalFree( lpMsgBuf );
		delete [] blanks;
        throw m_LastError;
        }    

    // Append blanks to end of file

    SetFilePointer(hF, 0, NULL, FILE_END); 
    DWORD written;
    WriteFile(hF,(LPCVOID)blanks, blank_len, &written, NULL);
    delete [] blanks;

    CloseHandle(hF);

    data.dataSIZE += blank_len;
    fact.samplesNumber += sample_count;
    }


bool CWaveDisk::AddWave(const CWave& w)
    {
	if (!m_Valid)
		throw m_LastError;

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

    // Open the wave file
    HANDLE hF = CreateFile(fileName.data(),GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
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
        AfxMessageBox((LPTSTR) lpMsgBuf);
        LocalFree( lpMsgBuf );
        return false;
        }  
    
    // Append new data to end of file

    SetFilePointer(hF, 0, NULL, FILE_END); 
    DWORD written;
    WriteFile(hF, w.wave, w.data.dataSIZE, &written, NULL);

    CloseHandle(hF);

    data.dataSIZE += w.data.dataSIZE;
    fact.samplesNumber += w.fact.samplesNumber;  

    return true;
    }

CString CWaveDisk::getFileName()
    {
	if (m_Valid)
		return fileName.c_str();
	else
		return CString("");
    }

float CWaveDisk::getDuration() 
	{
	if (!m_Valid)
		throw m_LastError;
	else
		return ((float)fact.samplesNumber)/(float)(fmt.nSamplesPerSec);
	}