 // Wave.cpp: implementation of the CWave class.
//
//////////////////////////////////////////////////////////////////////

#include "Wave.hpp"
#include <queue>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <iostream>


const unsigned int c_sample_rate = 22050;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWave::CWave(string _fileName) throw (string) : fileName(_fileName), m_Valid(true)
    {
    extraParam = NULL;
    wave=NULL;
    extraParamLength = 0;
    fact.samplesNumber=-1;
    int hF = open(fileName.c_str(), O_RDONLY);
    if (hF==-1)
        {
		m_LastError = "Error opening " + _fileName + ". error: " + string((char*)strerror(errno));
		m_Valid = false;
		return;
        }
        
    DWORD readCnt;
    readCnt = read(hF,(LPVOID)&riff,RIFF_SIZE);
    if (readCnt != RIFF_SIZE)
		{
		close(hF);
        m_LastError = "Couldn't read RIFF data in wav file.";
        m_Valid = false;
        return;	
		}
    readCnt = read(hF,(LPVOID)&fmt, FMT_SIZE - 2);

    if (fmt.wFormatTag!=1)
        {
        close(hF);
        m_LastError = "Unable to handle this format - can only handle format 1 (PCM) files.";
        m_Valid = false;
        return;
        }
    if (fmt.fmtSIZE != FMT_SIZE -8 && fmt.fmtSIZE != FMT_SIZE -2 -8)
        {
        close(hF);
        m_LastError = "Unable to handle this format - fmt chunk is unexpected length.";
        m_Valid = false;
        return;
        }
    if (fmt.wBitsPerSample != 16)
        {
        close(hF);
        m_LastError = "Unable to handle this format - only 16 bits/sample supported.";
        m_Valid = false;
        return;
        }
    if (fmt.nSamplesPerSec != 22050)
        {
        m_LastError = "Unable to handle this format - only 22050 Hz supported.";
        close(hF);
        m_Valid = false;
        return;
        }
    if (fmt.nAvgBytesPerSec != 44100)
        {
        close(hF);
        m_LastError = "Unable to handle this format - only 44100 Bytes/sec supported.";
        m_Valid = false;
        return;
        }
    if (fmt.nChannels != 1)
        {
        close(hF);
        m_LastError = "Unable to handle this format - only single channel (mono) is supported.";
        m_Valid = false;
        return;
        }
	if (fmt.fmtSIZE == FMT_SIZE -8)  
		{
		char buf[10];
		// Need to read (and throw away) the wExtraBits field
		readCnt = read(hF,(LPVOID)&buf, 2);
		}

    bool needFactFixup(false);
    readCnt = read(hF,(LPVOID)&fact.factID,4);

    if (strncmp(fact.factID, "fact", 4))
        {
        if (!strncmp(fact.factID, "data", 4))
            {
            strncpy(data.dataID, fact.factID, 4);
            strncpy(fact.factID, "fact", 4);
            fact.factSIZE = FACT_SIZE - 8;
            needFactFixup = true;
            }
        else
            {
            close(hF);
            m_LastError = "Unable to handle this format - unexpected chunk found when expecting fact or data.";
            m_Valid = false;
            return;
            }
        }
    else
        {
        readCnt = read(hF,(LPVOID)&fact.factSIZE,FACT_SIZE-4);
        }

    if (fact.factSIZE != FACT_SIZE - 8)
        {
        close(hF);
        m_LastError = "Unable to handle this format - fact chunk is unexpected length.";
        m_Valid = false;
        return;
        }


    if (needFactFixup) //  found data chunk when looking for fact chunk
        {
        readCnt = read(hF,(LPVOID)&data.dataSIZE,DATA_SIZE-4);
        fact.samplesNumber = data.dataSIZE / (fmt.wBitsPerSample/8);  // nb, should be 2 byte per sample, given other restrictions
        }
    else
        {
        readCnt = read(hF,(LPVOID)&data,DATA_SIZE);
        }

    if (fmt.nSamplesPerSec != (c_sample_rate)) //  /fmt.wBitsPerSample))
        {
        close(hF);
        m_LastError = "Unable to use this file, sampling rate in file is not 22050Hz (required).";
        m_Valid = false;
        return;
        }

 /*   wave = new BYTE[data.dataSIZE];
    bufSize = data.dataSIZE;
    read(hF,(LPVOID)wave,data.dataSIZE,&readCnt,NULL);    
*/
    close(hF);
}

float CWave::getDuration()
    {
    if (m_Valid) return ((float)fact.samplesNumber)/(float)(fmt.nSamplesPerSec);
    throw m_LastError;
    }

CWave::~CWave(){
    if (wave)
        delete[] wave;
}





