// Countdown.h : main header file for the COUNTDOWN application
//

#if !defined(AFX_COUNTDOWN_H__E1E8C7AD_758E_4E39_9191_EC0CA1227688__INCLUDED_)
#define AFX_COUNTDOWN_H__E1E8C7AD_758E_4E39_9191_EC0CA1227688__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#pragma  warning(disable: 4786)
#include "RecordSound.h"
#include "RecordCollection.h"
 

/////////////////////////////////////////////////////////////////////////////
// CCountdownApp:
// See Countdown.cpp for the implementation of this class
//

class CCountdownApp : public CWinApp
{
public:
	CCountdownApp();
    CString CustomDir() {return CString(szAppDataPath);};
    CString CustomSoundDir() {return CString (szAppDataSoundPath);};
	CString ConfigFile() {return CString (szAppFilePath);};
	void SetNotifyHwnd(HWND a_Hwnd);
	void Reload();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCountdownApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCountdownApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    CRecordCollection m_Recs;

	protected:
		BOOL m_Recording;
		CRecordSound* m_RecordSound;
		CWriteSoundFile* m_WriteSound;
		
	protected:
		BOOL InitRecording();
		BOOL InitPlaying();
		BOOL InitWriting();
		BOOL InitPlayMMSound();


        char szAppDataPath[MAX_PATH];
		char szAppFilePath[MAX_PATH];
        char szAppDataSoundPath[MAX_PATH];
	
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COUNTDOWN_H__E1E8C7AD_758E_4E39_9191_EC0CA1227688__INCLUDED_)
