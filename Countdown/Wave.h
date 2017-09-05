  // Wave.h: interface for the CWave class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVE_H__5DF5361A_4047_40DF_A13A_5A5CA8971DEA__INCLUDED_)
#define AFX_WAVE_H__5DF5361A_4047_40DF_A13A_5A5CA8971DEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <string>
#include "Windows.h"
using namespace std;

#define RIFF_SIZE 12
#define FMT_SIZE 26
#define DATA_SIZE 8
#define FACT_SIZE 12
class CWave
{
friend class CWaveDisk;
public:
    static string getFileTitle(string fullPath);
    CString getFileName();
    void setFileName(string _fileName);
    void saveToFile();
    CWave(CString _fileName);
    CWave(const CWave& w);
    CWave();
    virtual ~CWave();
    float getDuration();
    void getAmplitude(short& a_Peak, short& a_Avg);
    void Normalise(float a_BoostFactor);
    void setExpectedMaxDuration(float a_Duration);
    bool IsValid() {return m_Valid;};
    CString LastError() {return m_LastError;};

    //CWave operator+(const CWave& wave) const throw (LPCSTR) ;
    CWave& operator= (const CWave &w);
    virtual void AddBlank(float a_Duration);
    virtual bool AddWave(const CWave& w);
    typedef struct _RIFF{
        char riffID[4]; //4
        DWORD riffSIZE; //4
        char riffFORMAT[4];//4

    }RIFF ;

    typedef struct _FMT{
        char fmtID[4]; //4
        DWORD fmtSIZE; //4

        WORD wFormatTag; //2
        WORD nChannels; //2
        DWORD nSamplesPerSec; //4
        DWORD nAvgBytesPerSec; //4
        WORD nBlockAlign; //2
        WORD wBitsPerSample; //2
        WORD wExtraBits;  // 2 bytes (optional)

    }FMT;

    typedef struct _DATA{
        char dataID[4]; //4 bytes
        DWORD dataSIZE; // 4 bytes
    }DATA ;

    typedef struct _FACT{
        char factID[4]; //4
        DWORD factSIZE; //4
        DWORD samplesNumber;
    }FACT;

    static string getFileFolder(string fullPath);
protected:

    void init(const CWave&);
    BYTE* wave;
    RIFF riff;
    FMT fmt;
    FACT fact;
    DATA data;
    string fileName;
    WORD extraParamLength;
    BYTE* extraParam;
    DWORD bufSize;
    bool m_Valid;
    CString m_LastError;
};

#endif // !defined(AFX_WAVE_H__5DF5361A_4047_40DF_A13A_5A5CA8971DEA__INCLUDED_)
