  // WaveDisk.h: interface for the CWaveDisk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CWAVEDISK)
#define CWAVEDISK

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <string>
#include "Windows.h"
#include "Wave.h"
using namespace std;

#define RIFF_SIZE 12
#define FMT_SIZE 26
#define DATA_SIZE 8
#define FACT_SIZE 12
class CWaveDisk 
{
public:
    void saveToFile();
    CWaveDisk(CString _fileName);
    CString getFileName();
	bool IsValid() {return m_Valid;};
	CString LastError() {return m_LastError;};

    virtual ~CWaveDisk();
    float getDuration();
    void AddBlank(float a_Duration);
    bool AddWave(const CWave& w);

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
        WORD wExtraBits;

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
private:
    // Hide 
    CWaveDisk(const CWaveDisk& w);
    CWaveDisk();

    RIFF riff;
    FMT fmt;
    FACT fact;
    DATA data;
    string fileName;
    WORD extraParamLength;
    BYTE* extraParam;
    bool factChunkFound;
	bool m_Valid;
	CString m_LastError;
};

#endif // !defined(CWAVEDISKH)
