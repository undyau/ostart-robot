// RecordSound.cpp : implementation file
//

#include "stdafx.h"
#include <mmsystem.h>
//#include "record.h"
#include "RecordSound.h"
#include "WriteSoundFile.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecordSound

IMPLEMENT_DYNCREATE(CRecordSound, CWinThread)


#define MAXINPUTBUFFERS 5

CRecordSound::CRecordSound(int iHertz)
{
	
	
	memset(&m_WaveFormatEx,0x00,sizeof(m_WaveFormatEx));
	m_WaveFormatEx.wFormatTag = WAVE_FORMAT_PCM;
	m_WaveFormatEx.nChannels = 1;
	m_WaveFormatEx.wBitsPerSample = 16;
	m_WaveFormatEx.cbSize = 0;
	m_WaveFormatEx.nSamplesPerSec = iHertz;//22050;
	m_WaveFormatEx.nAvgBytesPerSec = m_WaveFormatEx.nSamplesPerSec
		*(m_WaveFormatEx.wBitsPerSample/8);
	m_WaveFormatEx.nBlockAlign = 
		(m_WaveFormatEx.wBitsPerSample/8)*
		m_WaveFormatEx.nChannels;

	m_bRecording = FALSE;
	m_Writer = NULL;
}






CRecordSound::~CRecordSound()
{
}

BOOL CRecordSound::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CRecordSound::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CRecordSound, CWinThread)
	//{{AFX_MSG_MAP(CRecordSound)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(WM_RECORDSOUND_STARTRECORDING, StartRecord)
	ON_THREAD_MESSAGE(WM_RECORDSOUND_STOPRECORDING, StopRecord)
	ON_THREAD_MESSAGE(MM_WIM_DATA, OnSoundBlock)
	ON_THREAD_MESSAGE(WM_RECORDSOUND_ENDTHREAD, OnEndThread)
	ON_THREAD_MESSAGE(WM_RECORDSOUND_WRITERTHREAD,OnPtrSoundWriter)
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CRecordSound message handlers
void CRecordSound::StartRecord(WPARAM wParam, LPARAM lParam)
{

	DWORD mmReturn = 0;

	if(m_bRecording)
		return;

    CString* fname = reinterpret_cast<CString*>(lParam);
    CString filename = *fname;
    delete fname;

	if(!m_bRecording)
	{
		// open wavein device
		MMRESULT mmReturn = ::waveInOpen( &m_hRecord, WAVE_MAPPER,
			&m_WaveFormatEx, ::GetCurrentThreadId(), 0, CALLBACK_THREAD);
		if(mmReturn )
		{
			char errorbuffer[MAX_PATH];
			char errorbuffer1[MAX_PATH];
			waveInGetErrorText( mmReturn, 
								errorbuffer,
								MAX_PATH);
			sprintf_s(errorbuffer1, sizeof(errorbuffer1), "WAVEIN:%x:%s",mmReturn,errorbuffer);

			AfxMessageBox(errorbuffer1);  

		}
	
			
		if(!mmReturn)
		{
			AllocateBuffers(MAXINPUTBUFFERS);
			mmReturn = ::waveInStart(m_hRecord);
			if(mmReturn )
		    {
			    char errorbuffer[MAX_PATH];
			    char errorbuffer1[MAX_PATH];
			    waveInGetErrorText( mmReturn, 
								    errorbuffer,
								    MAX_PATH);
			    sprintf_s(errorbuffer1, sizeof(errorbuffer1), "WAVEIN:%x:%s",mmReturn,errorbuffer);

			    AfxMessageBox(errorbuffer1);  

		    }
	
		    if(!mmReturn)
		    {
			    m_bRecording = TRUE;
			    //if(m_Player)
				//    m_Player->PostThreadMessage(WM_PLAYSOUND_STARTPLAYING, 0, 0);				
				if(m_Writer)
				{
					PWRITESOUNDFILE pwsf= (PWRITESOUNDFILE) new WRITESOUNDFILE;
					ZeroMemory(pwsf,sizeof(WRITESOUNDFILE));
					char *p = pwsf->lpszFileName;
					strcpy_s(p, sizeof(pwsf->lpszFileName), filename.operator LPCTSTR());
					memcpy(&pwsf->waveFormatEx,&m_WaveFormatEx,sizeof(m_WaveFormatEx));
					m_Writer->PostThreadMessage(WM_WRITESOUNDFILE_FILENAME,0,(LPARAM)pwsf);
				}
	    
			    return;
		    }
		}
	

	}
	return;
}
void CRecordSound::AllocateBuffers(int nBuffers)
{
    int i;
	for(i=0; i < nBuffers; i++)
	{
		LPWAVEHDR lpWaveHdr = CreateWaveHeader();
		::waveInPrepareHeader(m_hRecord,lpWaveHdr, sizeof(WAVEHDR));
		::waveInAddBuffer(m_hRecord, lpWaveHdr, sizeof(WAVEHDR));			
	}
}
void CRecordSound::FreeSpareBuffers()
{
		
	MSG msg;
	Sleep(20);
	while(::PeekMessage(&msg,NULL,MM_WIM_OPEN,MM_WIM_DATA,
					PM_REMOVE))
	{
	
	  if(msg.wParam == MM_WIM_OPEN || msg.wParam == MM_WIM_CLOSE)
		  continue;
	  LPWAVEHDR lpWaveHdr = (LPWAVEHDR) msg.lParam;
	  if(lpWaveHdr)
	  {
		
		BYTE * lpByte = (BYTE*) lpWaveHdr->lpData;
		::waveInUnprepareHeader(m_hRecord, lpWaveHdr, sizeof(WAVEHDR));
		if(lpByte)
			delete lpByte;
		delete lpWaveHdr;
	  }
	}

}

void CRecordSound::StopRecord(WPARAM wParam, LPARAM lParam)
{
	MMRESULT mmReturn = 0;
	if(!m_bRecording)
		return;

	if(m_bRecording)
	{
		mmReturn = ::waveInStop(m_hRecord);
		if(!mmReturn)
			mmReturn = ::waveInReset(m_hRecord);

		FreeSpareBuffers();

		if(!mmReturn)
			m_bRecording = FALSE;
		Sleep(500);
		if(!mmReturn)
			mmReturn = ::waveInClose(m_hRecord);

		if(m_Writer)
			m_Writer->PostThreadMessage(WM_WRITESOUNDFILE_CLOSEFILE,0,0);
		
		return;
	}
	return;
}
void CRecordSound::OnSoundBlock(WPARAM wParam, LPARAM lParam)
{
	LPWAVEHDR lpWaveHdr = (LPWAVEHDR) lParam;

	if(lpWaveHdr)
	{
		BYTE * lpInt = (BYTE*) lpWaveHdr->lpData;
		DWORD iRecorded = lpWaveHdr->dwBytesRecorded;
		::waveInUnprepareHeader(m_hRecord, lpWaveHdr, sizeof(WAVEHDR));

		ProcessSoundData(lpInt, iRecorded/sizeof(BYTE));
		if(m_Writer)
		{
			WAVEHDR* pWriteHdr = new WAVEHDR;
			if(!pWriteHdr)
				return;
			memcpy(pWriteHdr,lpWaveHdr,sizeof(WAVEHDR));
			BYTE * pSound = new BYTE[lpWaveHdr->dwBufferLength];
			if(!pSound)
			{
				delete pWriteHdr;
				return;
			}
			memcpy(pSound,lpWaveHdr->lpData,lpWaveHdr->dwBufferLength);
			pWriteHdr->lpData = (char*)pSound;
			m_Writer->PostThreadMessage(WM_WRITESOUNDFILE_WRITEBLOCK,GetCurrentThreadId(),(LPARAM) pWriteHdr);
		}
		

	//	if(m_Player)
	//	    {
	//		m_Player->PostThreadMessage(WM_PLAYSOUND_PLAYBLOCK, GetCurrentThreadId(),(LPARAM) lpWaveHdr);		
	//	    }
	//	else
	//	{
			delete lpInt;
			delete lpWaveHdr;
	//	}
		char debugbuffer[256];
		sprintf_s(debugbuffer, sizeof(debugbuffer),  "SOUND BUFFER returned: %d\n",iRecorded);
		TRACE(debugbuffer);
		if(m_bRecording)
		    {
			AllocateBuffers(1);
		    }
	}


	return;
}

LPWAVEHDR CRecordSound::CreateWaveHeader()
{
	LPWAVEHDR lpWaveHdr = new WAVEHDR;
	ZeroMemory(lpWaveHdr, sizeof(WAVEHDR));
	BYTE* lpByte = new BYTE[(m_WaveFormatEx.nBlockAlign*SOUNDSAMPLES)];

	lpWaveHdr->lpData = (char *) lpByte;
	lpWaveHdr->dwBufferLength = (m_WaveFormatEx.nBlockAlign*SOUNDSAMPLES);
	return lpWaveHdr;

}

void CRecordSound::ProcessSoundData(BYTE* sound, DWORD dwSamples)
{
}

void CRecordSound::OnEndThread(WPARAM wParam, LPARAM lParam)
{
	if(m_bRecording)
	{
		StopRecord(0, 0);
	}
	::PostQuitMessage(0);
	return;
}

void CRecordSound::OnPtrSoundWriter(WPARAM wParam, LPARAM lParam)
{
	m_Writer = (CWriteSoundFile*) lParam;
	return;
}
	