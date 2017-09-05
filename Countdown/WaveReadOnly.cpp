 // Wave.cpp: implementation of the CWaveReadOnly class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaveReadOnly.h"

const int c_sample_rate = 22050;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWaveReadOnly::CWaveReadOnly(CString _fileName) : m_Valid(false)
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
		CloseHandle(hF);
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

    // Get current file position
    dataStartPosition = SetFilePointer( hF, 0, NULL, FILE_CURRENT);

    CloseHandle(hF);
	m_Valid = true;
    }


CWaveReadOnly::~CWaveReadOnly()
    {
    }


CString CWaveReadOnly::getFileName()
    {
	if (m_Valid)
		return fileName.c_str();
	else
		return CString("");
    }