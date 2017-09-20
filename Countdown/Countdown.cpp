// Countdown.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Countdown.h"
#include "CountdownDlg.h"
#include "Shlobj.h"
#include "utils.h"
#pragma warning(disable:4091)
#include "imagehlp.h"
#pragma warning(default:4091)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#  ifndef CSIDL_COMMON_APPDATA
#    define CSIDL_COMMON_APPDATA 35
#  endif

/////////////////////////////////////////////////////////////////////////////
// CCountdownApp

BEGIN_MESSAGE_MAP(CCountdownApp, CWinApp)
	//{{AFX_MSG_MAP(CCountdownApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
    ON_COMMAND( ID_HELP_INDEX, OnHelpIndex ) 
	
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CCountdownApp construction

CCountdownApp::CCountdownApp() 
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCountdownApp object

CCountdownApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCountdownApp initialization

BOOL CCountdownApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	CoInitialize(NULL);

	InitRecording();
	InitWriting();
	
	if(m_RecordSound)
	    {
		m_RecordSound->PostThreadMessage(WM_RECORDSOUND_WRITERTHREAD ,(WPARAM)0, (LPARAM)
			m_WriteSound);	
    	}
	
    SHGetSpecialFolderPath(NULL, szAppDataPath, CSIDL_COMMON_APPDATA, FALSE);
    
	strcat_s(szAppDataPath, sizeof(szAppDataPath), "\\Undy\\Countdown");
	strcpy_s(szAppDataSoundPath, 260, szAppDataPath);
    strcat_s(szAppDataSoundPath, sizeof(szAppDataSoundPath), "\\Custom");
	strcpy_s(szAppDataNamesPath, 260, szAppDataPath);
	strcat_s(szAppDataNamesPath, sizeof(szAppDataNamesPath), "\\Names");
	strcpy_s(szAppFilePath, sizeof(szAppFilePath), szAppDataPath);
    strcat_s(szAppFilePath, sizeof(szAppFilePath), "\\forAndy.xml");

    MakeSureDirectoryPathExists(szAppFilePath);
    
    char temp[MAX_PATH];
    strcpy_s(temp, sizeof(temp), szAppDataSoundPath);
    strcat_s(temp, sizeof(temp), "\\dummy.wav");
    MakeSureDirectoryPathExists(temp);  
	strcpy_s(temp, sizeof(temp), szAppDataNamesPath);
	strcat_s(temp, sizeof(temp), "\\dummy.wav");
	MakeSureDirectoryPathExists(temp);
    

    Reload();
	if (FileExists(m_Recs.GetStartListFile()))
		StartList().Load(m_Recs.GetStartListFile());

    CCountdownDlg dlg;
	m_pMainWnd = &dlg;

	dlg.m_RecordThread = m_RecordSound;

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CCountdownApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_Recs.SaveToXML(szAppFilePath);


	if(m_RecordSound)
	    {
		m_RecordSound->PostThreadMessage(WM_RECORDSOUND_ENDTHREAD,0,0);
		::WaitForSingleObject(m_RecordSound->m_hThread, 5000);
		m_RecordSound = NULL;
	    }
	if(m_WriteSound)
	    {
		m_WriteSound->PostThreadMessage(WM_WRITESOUNDFILE_ENDTHREAD,0,0);
		::WaitForSingleObject(m_WriteSound->m_hThread, 5000);
		m_WriteSound = NULL;
	    }   
	

	return CWinApp::ExitInstance();
}

BOOL CCountdownApp::InitRecording()
{
	m_RecordSound = new CRecordSound();
	m_RecordSound->CreateThread();
	return TRUE;
}


BOOL CCountdownApp::InitWriting()
{
	m_WriteSound = new CWriteSoundFile();
	m_WriteSound->CreateThread();
	return TRUE;
}

void CCountdownApp::SetNotifyHwnd(HWND a_Hwnd)
	{
	m_WriteSound->SetNotifyHwnd(a_Hwnd);
	}

void CCountdownApp::Reload()
	{
	m_Recs.ReloadFromXML(szAppFilePath);
	}

bool CCountdownApp::CreateSoundFile(CString const & a_Name, CString const & a_FileName)
	{
	return m_TTSGenerator.GenerateSound(a_Name, a_FileName);
	}
