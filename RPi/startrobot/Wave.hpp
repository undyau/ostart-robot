  // Wave.hpp: interface for the CWave class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVE_H__5DF5361A_4047_40DF_A13A_5A5CA8971DEA__INCLUDED_)
#define AFX_WAVE_H__5DF5361A_4047_40DF_A13A_5A5CA8971DEA__INCLUDED_

#include <string>
#include <stdint.h>

using namespace std;

#define RIFF_SIZE 12
#define FMT_SIZE 26
#define DATA_SIZE 8
#define FACT_SIZE 12

typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;
typedef void * LPVOID;

class CWave
{
public:
    CWave(string _fileName) throw (string) ;
    virtual ~CWave();
    float getDuration();
    bool isValid() {return m_Valid;}

#pragma pack(1)
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

#pragma pack(0)

protected:
    BYTE* wave;
    RIFF riff;
    FMT fmt;
    FACT fact;
    DATA data;
    string fileName;
    bool m_Valid;    
    WORD extraParamLength;
    BYTE* extraParam;
    DWORD bufSize;
	string m_LastError;
};

#endif // !defined(AFX_WAVE_H__5DF5361A_4047_40DF_A13A_5A5CA8971DEA__INCLUDED_)
