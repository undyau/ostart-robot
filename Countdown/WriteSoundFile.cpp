// WriteSoundFile.cpp : implementation file
//

#include "stdafx.h"
#include <mmsystem.h>
//#include "record.h"
#include "WriteSoundFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWriteSoundFile

IMPLEMENT_DYNCREATE(CWriteSoundFile, CWinThread)

CWriteSoundFile::CWriteSoundFile() : m_NotifyThreadHwnd(NULL)
{
}

CWriteSoundFile::~CWriteSoundFile()
{
}

BOOL CWriteSoundFile::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CWriteSoundFile::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CWriteSoundFile, CWinThread)
	//{{AFX_MSG_MAP(CWriteSoundFile)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(WM_WRITESOUNDFILE_FILENAME, CreateWaveFile)
	ON_THREAD_MESSAGE(WM_WRITESOUNDFILE_WRITEBLOCK, WriteToSoundFile)
	ON_THREAD_MESSAGE(WM_WRITESOUNDFILE_CLOSEFILE, CloseSoundFile)
	ON_THREAD_MESSAGE(WM_WRITESOUNDFILE_ENDTHREAD, OnEndThread)

END_MESSAGE_MAP()

void CWriteSoundFile::CreateWaveFile(WPARAM wParam, LPARAM lParam)
{
	PWRITESOUNDFILE pWriteSoundFile = (PWRITESOUNDFILE) lParam;
	int cbWaveFormatEx = sizeof(WAVEFORMATEX) + pWriteSoundFile->waveFormatEx.cbSize;

	m_hFile = ::mmioOpen(pWriteSoundFile->lpszFileName,NULL, MMIO_CREATE|MMIO_WRITE|MMIO_EXCLUSIVE | MMIO_ALLOCBUF);
	if(!m_hFile)
		return;

	ZeroMemory(&m_MMCKInfoParent, sizeof(MMCKINFO));
	m_MMCKInfoParent.fccType = mmioFOURCC('W','A','V','E');

	MMRESULT mmResult =  ::mmioCreateChunk( m_hFile,&m_MMCKInfoParent,
							MMIO_CREATERIFF);
	
	ZeroMemory(&m_MMCKInfoChild, sizeof(MMCKINFO));
	m_MMCKInfoChild.ckid = mmioFOURCC('f','m','t',' ');
	m_MMCKInfoChild.cksize = cbWaveFormatEx;
	mmResult = ::mmioCreateChunk(m_hFile, &m_MMCKInfoChild, 0);
	mmResult = ::mmioWrite(m_hFile, (char*)&pWriteSoundFile->waveFormatEx, cbWaveFormatEx); 
	mmResult = ::mmioAscend(m_hFile, &m_MMCKInfoChild, 0);
	m_MMCKInfoChild.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmResult = ::mmioCreateChunk(m_hFile, &m_MMCKInfoChild, 0);

	if(pWriteSoundFile)
		delete pWriteSoundFile;

	return;

}
void CWriteSoundFile::WriteToSoundFile(WPARAM wParam, LPARAM lParam)
{
	LPWAVEHDR lpHdr = (LPWAVEHDR) lParam;
	int cbLength = lpHdr->dwBufferLength;
	if(lpHdr)
	{
		char *soundbuffer = (char*) lpHdr->lpData;
		if(m_hFile && soundbuffer)
			::mmioWrite(m_hFile, soundbuffer, cbLength);
		if(soundbuffer)
			delete (BYTE*) soundbuffer;
		if(lpHdr)
			delete lpHdr;
	}

	return;
}

void CWriteSoundFile::CloseSoundFile(WPARAM wParam, LPARAM lParam)
{
	
	if(m_hFile)
	{
		::mmioAscend(m_hFile, &m_MMCKInfoChild, 0);
		::mmioAscend(m_hFile, &m_MMCKInfoParent, 0);
		::mmioClose(m_hFile, 0);
		m_hFile = NULL;
		if (m_NotifyThreadHwnd)
			::SendMessage(m_NotifyThreadHwnd,WM_WRITESOUNDFILE_CLOSEDFILE,0,0);
	}
	
	return;
}
void CWriteSoundFile::OnEndThread(WPARAM wParam, LPARAM lParam)
{
	CloseSoundFile(0,0);
	::PostQuitMessage(0);
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CWriteSoundFile message handlers
