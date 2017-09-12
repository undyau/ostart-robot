// CountdownDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Countdown.h"
#include "CountdownDlg.h"
#include "Recording.h"
#include "Wave.h"
#include "ProgressWnd.h"
#include "utils.h"
#include "RecordTimesDlg.h"
#include "MaintenanceDlg.h"
#include "TimeRecording.h"
#include "CustomRecording.h"
#include "XBrowseForFolder.h"
#pragma warning(disable:4091)
#include "imagehlp.h"
#pragma warning(default:4091)
#include "RecordingPropsDlg.h"


extern CCountdownApp theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
	{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_GnuText;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
	{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_GnuText = _T("");
	//}}AFX_DATA_INIT
	}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
	{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_STATIC_GNU, m_GnuText);
	//}}AFX_DATA_MAP
	}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_GnuText = "This software uses the lame_enc library under the terms\nof the GNU Lesser Public License. The license text is \n";
    m_GnuText += "distributed with this program in file lesser.txt.\n\n",
    m_GnuText += "Section 6(b) of the license applies to this application\n";
    m_GnuText += "The user may use any version of the lame_enc.dll in place\n";
    m_GnuText += "of the supplied version. \n\n";
    m_GnuText += "problems - keep the supplied version somewhere safe !).\n\n";
    m_GnuText += "The source for lame_enc.dll is available from\n";
    m_GnuText += "http://lame.sourceforge.net or\nhttp://www.mp3dev.org\n\n";
    m_GnuText += "lame_enc.dll © The Lame Project 2003";

    UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCountdownDlg dialog

CCountdownDlg::CCountdownDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCountdownDlg::IDD, pParent), m_InAdd(false), m_InCreate(false), m_InExit(false),
    m_InterimChange(false), m_ModifyMode(false)
	, m_StartListFile(_T(""))
	, m_StartersPlusMM(_T(""))
{
	//{{AFX_DATA_INIT(CCountdownDlg)
	m_ContentMsg = _T("");
	m_FreqMsg = _T("00");
	m_FreqMM = _T("01");
	m_NewMsg = _T("");
	m_LastHH = _T("14");
	m_LastMM = _T("03");
	m_FirstHH = _T("14");
	m_FirstMM = _T("00");
	m_ContentRadio = 0;
	m_FreqRadio = 0;
	m_TimingRadio = 0;
	m_FileName = _T("");
	m_TimingMM = _T("00");
	m_TimingSS = _T("00");
	m_TimingTT = _T("0");
	m_NextPlusMM = _T("");
	m_FreqSS = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    char szAppPath[MAX_PATH] = "";
    CString strAppName;
    ::GetModuleFileName(0, szAppPath, MAX_PATH);

    strAppName = szAppPath;
    strAppName = strAppName.Left(strAppName.ReverseFind('\\'));
	}

void CCountdownDlg::DoDataExchange(CDataExchange* pDX)
	{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCountdownDlg)
	DDX_Control(pDX, ID_CANCEL_BTN, m_CancelBtnCtrl);
	DDX_Control(pDX, IDC_EDIT_TIMING_FINISH_TT, m_TimingTTCtrl);
	DDX_Control(pDX, IDC_EDIT_TIMING_FINISH_SS, m_TimingSSCtrl);
	DDX_Control(pDX, IDC_EDIT_NEXT_PLUS_MM, m_NextPlusMMCtrl);
	DDX_Control(pDX, IDC_EDIT_STARTERS_PLUS_MM, m_StartersPlusMMCtrl);
	DDX_Control(pDX, IDC_BUTTON_AMPLIFY, m_AmplifyCtrl);
	DDX_Control(pDX, IDC_BUTTON_DAMPEN, m_DampenCtrl);
	DDX_Control(pDX, IDC_BUTTON_MODIFY, m_ModifyCtrl);
	DDX_Control(pDX, IDC_BUTTON_ADDAFTER, m_AddAfterCtrl);
	DDX_Control(pDX, IDC_BUTTON_PLAYREC, m_PlayRecCtrl);
	DDX_Control(pDX, IDC_EDIT_TIMING_FINISH_MM, m_TimingMMCtrl);
	DDX_Control(pDX, IDC_COMBO_RELATIVE, m_RelativeCtrl);
	DDX_Control(pDX, IDC_EDIT_LAST_MM, m_LastMMCtrl);
	DDX_Control(pDX, IDC_EDIT_LAST_HH, m_LastHHCtrl);
	DDX_Control(pDX, IDC_EDIT_FIRST_MM, m_FirstMMCtrl);
	DDX_Control(pDX, IDC_EDIT_FIRST_HH, m_FirstHHCtrl);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_AddBtnCtrl);
	DDX_Control(pDX, IDC_BUTTON_PLAY, m_PlayCtrl);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_DeleteCtrl);
	DDX_Control(pDX, IDC_BUTTON_CREATE, m_CreateCtrl);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_BrowseCtrl);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_FileNameCtrl);
	DDX_Control(pDX, IDC_EDIT_STARTLISTFILE, m_StartListCtrl);
	DDX_Control(pDX, IDC_DYN_LED, m_LedCtrl);
	DDX_Control(pDX, IDC_RADIO_FREQ_EVERY, m_FreqRadioCtrl);
	DDX_Control(pDX, IDC_RADIO_CONTENT_MSG, m_ContentRadioCtrl);
	DDX_Control(pDX, IDC_BUTTON_RECORD, m_RecordingCtrl);
	DDX_Control(pDX, IDC_LIST_MESSAGES, m_ListMsgs);
	DDX_Control(pDX, IDC_COMBO_RECORD_MSG, m_NewMsgCtrl);
	DDX_Control(pDX, IDC_COMBO_CONTENT_MSG, m_ContentMsgCtrl);
	DDX_Control(pDX, IDC_EDIT_FREQ_SS, m_FreqSSCtrl);
	DDX_Control(pDX, IDC_EDIT_FREQ_MM, m_FreqMMCtrl);
	DDX_Control(pDX, IDC_COMBO_FREQ_MSG, m_FreqMsgCtrl);
	DDX_CBString(pDX, IDC_COMBO_CONTENT_MSG, m_ContentMsg);
	DDX_CBString(pDX, IDC_COMBO_FREQ_MSG, m_FreqMsg);
	DDX_Text(pDX, IDC_EDIT_FREQ_MM, m_FreqMM);
	DDV_MaxChars(pDX, m_FreqMM, 3);
	DDX_CBString(pDX, IDC_COMBO_RECORD_MSG, m_NewMsg);
	DDX_Text(pDX, IDC_EDIT_LAST_HH, m_LastHH);
	DDV_MaxChars(pDX, m_LastHH, 2);
	DDX_Text(pDX, IDC_EDIT_LAST_MM, m_LastMM);
	DDV_MaxChars(pDX, m_LastMM, 2);
	DDX_Text(pDX, IDC_EDIT_FIRST_HH, m_FirstHH);
	DDV_MaxChars(pDX, m_FirstHH, 2);
	DDX_Text(pDX, IDC_EDIT_FIRST_MM, m_FirstMM);
	DDV_MaxChars(pDX, m_FirstMM, 2);
	DDX_Radio(pDX, IDC_RADIO_CONTENT_MSG, m_ContentRadio);
	DDX_Radio(pDX, IDC_RADIO_FREQ_EVERY, m_FreqRadio);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_FileName);
	DDX_Text(pDX, IDC_EDIT_STARTLISTFILE, m_StartListFile);
	DDX_Text(pDX, IDC_EDIT_TIMING_FINISH_MM, m_TimingMM);
	DDV_MaxChars(pDX, m_TimingMM, 2);
	DDX_Text(pDX, IDC_EDIT_TIMING_FINISH_SS, m_TimingSS);
	DDV_MaxChars(pDX, m_TimingSS, 2);
	DDX_Text(pDX, IDC_EDIT_TIMING_FINISH_TT, m_TimingTT);
	DDV_MaxChars(pDX, m_TimingTT, 1);
	DDX_Text(pDX, IDC_EDIT_NEXT_PLUS_MM, m_NextPlusMM);
	DDV_MaxChars(pDX, m_NextPlusMM, 3);
	DDX_Text(pDX, IDC_EDIT_FREQ_SS, m_FreqSS);
	DDV_MaxChars(pDX, m_FreqSS, 2);
	//}}AFX_DATA_MAP

	if (m_InAdd)
	{
		DDV_MinMaxInt(pDX, atoi(m_TimingMM), 0, 59);
		DDV_MinMaxInt(pDX, atoi(m_TimingSS), 0, 59);

		while (m_TimingMM.GetLength() < 2) m_TimingMM = " " + m_TimingMM;
		while (m_TimingSS.GetLength() < 2) m_TimingSS = " " + m_TimingSS;


		if (pDX->m_bSaveAndValidate)
		{
			if (m_ContentMsg.IsEmpty() && IsChecked(IDC_RADIO_CONTENT_MSG))
			{
				pDX->PrepareEditCtrl(IDC_COMBO_CONTENT_MSG);
				CString message;
				message.Format("A message must be selected");
				AfxMessageBox(message, MB_ICONINFORMATION, 0);
				pDX->Fail();
			}
		}
	}
	else if (!m_InExit)
	{
		DDV_MinMaxInt(pDX, atoi(m_FirstHH), 0, 23);
		DDV_MinMaxInt(pDX, atoi(m_FirstMM), 0, 59);

		DDV_MinMaxInt(pDX, atoi(m_LastHH), 0, 23);
		DDV_MinMaxInt(pDX, atoi(m_LastMM), 0, 59);

		DDV_MinMaxInt(pDX, atoi(m_FreqMM), 0, 179);
		DDV_MinMaxInt(pDX, atoi(m_FreqSS), 0, 59);

		while (m_FreqMM.GetLength() < 1) m_FreqMM = "0" + m_FreqMM;
		while (m_FreqSS.GetLength() < 2) m_FreqSS = "0" + m_FreqSS;
		if (pDX->m_bSaveAndValidate)
		{
			theApp.m_Recs.Frequency(m_FreqMM + ':' + m_FreqSS);
		}

		if (m_InCreate)
		{
			long start, finish;
			start = atoi(m_FirstHH) * 100 +
				atoi(m_FirstMM);

			finish = atoi(m_LastHH) * 100 +
				atoi(m_LastMM);

			if (start > finish)
			{
				pDX->PrepareEditCtrl(IDC_EDIT_FIRST_HH);
				CString message;
				message.Format("Start must be before end.");
				AfxMessageBox(message, MB_ICONINFORMATION, 0);
				pDX->Fail();
			}
		}
		if (!m_InterimChange)
		{
			DDV_PCFileName(pDX, IDC_EDIT_FILENAME, m_FileName);
			DDV_PCFileName(pDX, IDC_EDIT_STARTLISTFILE, m_StartListFile);
		}
	}


}

BEGIN_MESSAGE_MAP(CCountdownDlg, CDialog)
	//{{AFX_MSG_MAP(CCountdownDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CREATE, OnButtonCreate)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, OnButtonRecord)
	ON_BN_CLICKED(IDC_RADIO_CONTENT_MSG, OnRadioContentMsg)
	ON_BN_CLICKED(IDC_RADIO_CONTENT_TIME, OnRadioContentTime)
	ON_BN_CLICKED(IDC_RADIO_FREQ_AFTER_MSG, OnRadioFreqAfterMsg)
	ON_BN_CLICKED(IDC_RADIO_FREQ_EVERY, OnRadioFreqEvery)
	ON_BN_CLICKED(IDC_RADIO_TIMING_FINISH, OnRadioTimingFinish)
	ON_BN_CLICKED(IDC_RADIO_TIMING_START, OnRadioTimingStart)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_CBN_SELCHANGE(IDC_COMBO_FREQ_MSG, OnSelchangeComboFreqMsg)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_EN_CHANGE(IDC_EDIT_FILENAME, OnChangeEditFilename)
	ON_CBN_EDITCHANGE(IDC_COMBO_RECORD_MSG, OnEditchangeComboRecordMsg)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MESSAGES, OnClickListMessages)
	ON_NOTIFY(LVN_DELETEITEM, IDC_LIST_MESSAGES, OnDeleteitemListMessages)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST_MESSAGES, OnSetfocusListMessages)
	ON_BN_CLICKED(IDC_BUTTON_AMPLIFY, OnButtonAmplify)
	ON_CBN_SELCHANGE(IDC_COMBO_RECORD_MSG, OnSelchangeComboRecordMsg)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	ON_CBN_SELCHANGE(IDC_COMBO_RELATIVE, OnSelchangeComboRelative)
	ON_EN_CHANGE(IDC_EDIT_FREQ_HH, OnChangeEditFreqHh)
	ON_EN_CHANGE(IDC_EDIT_FREQ_MM, OnChangeEditFreqMm)
	ON_EN_CHANGE(IDC_EDIT_FREQ_SS, OnChangeEditFreqSs)
	ON_COMMAND(ID_MENU_ABOUT, OnMenuAbout)
	ON_COMMAND(ID_MENU_CLOCK, OnMenuClock)
	ON_COMMAND(ID_MENU_CREATE, OnMenuCreate)
	ON_COMMAND(ID_MENU_EXIT, OnMenuExit)
	ON_COMMAND(ID_MENU_HELP_CONTENTS, OnMenuHelpContents)
	ON_COMMAND(ID_MENU_MAINTENANCE, OnMenuMaintenance)
	ON_WM_CLOSE()
    ON_WM_INITMENUPOPUP()
	ON_BN_CLICKED(IDC_BUTTON_PLAYREC, OnButtonPlayrec)
	ON_BN_CLICKED(IDC_BUTTON_ADDAFTER, OnButtonAddafter)
	ON_BN_CLICKED(IDC_BUTTON_DAMPEN, OnButtonDampen)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, OnButtonModify)
	ON_COMMAND(ID_MENU_EXPORT, OnMenuExport)
	ON_COMMAND(ID_MENU_IMPORT, OnMenuImport)
	ON_BN_CLICKED(ID_CANCEL_BTN, OnCancelBtn)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WRITESOUNDFILE_CLOSEDFILE, OnClosedSoundFile)
	ON_COMMAND(ID_MENU_RECORDING, &CCountdownDlg::OnMenuRecordingProperties)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_STARTLIST, &CCountdownDlg::OnBnClickedButtonBrowseStartlist)
	ON_EN_CHANGE(IDC_EDIT_STARTLISTFILE, &CCountdownDlg::OnEnChangeEditStartlistfile)
	ON_BN_CLICKED(IDC_RADIO_CONTENT_STARTERS, &CCountdownDlg::OnBnClickedRadioContentStarters)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE, &CCountdownDlg::OnBnClickedButtonGenerate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCountdownDlg message handlers

BOOL CCountdownDlg::OnInitDialog()
	{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_Recording = false;


// Set the menu
    m_Menu.LoadMenu(IDR_MAIN_MENU);
    ASSERT(m_Menu);
    SetMenu(&m_Menu);

// create, initialize, and hook up image list
     int cx, cy;

    cx= GetSystemMetrics(SM_CXSMICON);
    cy= GetSystemMetrics(SM_CYSMICON);

    m_ImageList.Create(cx,cy, ILC_MASK, 8, 8);

    m_ImageList.Add(theApp.LoadIcon(IDI_ICON_TIME));
    m_ImageList.Add(theApp.LoadIcon(IDI_ICON_AFTER));
	m_ImageList.Add(theApp.LoadIcon(IDI_ICON_MSG));
	m_ImageList.Add(theApp.LoadIcon(IDI_ICON_MSG_AFTER));
	m_ImageList.Add(theApp.LoadIcon(IDI_ICON_TIME_AFTER));
    m_ListMsgs.SetImageList(&m_ImageList, LVSIL_SMALL);

    m_ListMsgs.InsertColumn(0,"Name", LVCFMT_LEFT, 3*m_ListMsgs.GetStringWidth("Really Long Name")/2);
    m_ListMsgs.InsertColumn(1,"Max Duration", LVCFMT_RIGHT, 3*m_ListMsgs.GetStringWidth("Max Duration")/2);
    m_ListMsgs.InsertColumn(2,"Timing", LVCFMT_CENTER, 5*m_ListMsgs.GetStringWidth("A long message about Timing")/2);

    // Enable the full row selection and the drag drop of headers.
    DWORD styles = LVS_EX_FULLROWSELECT;
    // Use macro since this is new and not in MFC.
    ListView_SetExtendedListViewStyleEx(m_ListMsgs.GetSafeHwnd(), styles, styles );

    m_LedCtrl.SetColor(RGB(255,0,0), RGB(100, 40, 40));
    m_LedCtrl.SetBlink(0);
    m_LedCtrl.SetOnOff(false);
    CRect rect;
    m_LedCtrl.GetWindowRect(rect);
    ScreenToClient(rect);
    rect.right = rect.left + 12;
    rect.bottom = rect.top + 12;
    m_LedCtrl.MoveWindow(rect);

    ShrinkWindow(IDC_STATIC_PUNC1);
    ShrinkWindow(IDC_STATIC_PUNC2);
  //  ShrinkWindow(IDC_STATIC_PUNC3);
  //  ShrinkWindow(IDC_STATIC_PUNC4);
    ShrinkWindow(IDC_STATIC_PUNC5);

    // Reload any stored stuff, in time order
	InitialiseData();

	return TRUE;  // return TRUE  unless you set the focus to a control
	}

void CCountdownDlg::InitialiseData()
	{
	m_FileName = theApp.m_Recs.GetFileName();
	m_StartListFile = theApp.m_Recs.GetStartListFile();
	m_LastHH = Word(theApp.m_Recs.StopTime(), 0, ':');
	m_LastMM = Word(theApp.m_Recs.StopTime(), 1, ':');
	m_FirstHH = Word(theApp.m_Recs.StartTime(), 0, ':');
	m_FirstMM = Word(theApp.m_Recs.StartTime(), 1, ':');
	m_FreqMM = Word(theApp.m_Recs.Frequency(), 0, ':');
	m_FreqSS = Word(theApp.m_Recs.Frequency(), 1, ':');
	UpdateData(FALSE);

	RedrawList();

	PopulateCombos();
    if (m_FreqMsgCtrl.GetCount() > 0)
        {
        m_FreqMsgCtrl.SetCurSel(0);
        m_ContentMsgCtrl.SetCurSel(0);
        }
    m_RelativeCtrl.SetCurSel(0);


	EnableCtrls();
	}

void CCountdownDlg::ShrinkWindow(int a_DlgID)
    {
    CRect rect;
    GetDlgItem(a_DlgID)->GetWindowRect(rect);
    ScreenToClient(rect);
    rect.right = rect.left + 3;
    GetDlgItem(a_DlgID)->MoveWindow(rect);
    }

void CCountdownDlg::OnSysCommand(UINT nID, LPARAM lParam)
	{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
	}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCountdownDlg::OnPaint() 
	{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCountdownDlg::OnQueryDragIcon()
	{
	return (HCURSOR) m_hIcon;
	}

void CCountdownDlg::OnButtonCreate() 
	{
	EnableCtrls();
    m_InCreate = true;
    if (!UpdateData(TRUE))
        {
        m_InCreate = false;
        return;
        }
    
    
    CProgressWnd wndProgress(this, "Recording Creation");  
    //wndProgress.GoModal(); // Call this if you want a modal window
    this->EnableWindow(false);
    theApp.m_Recs.SetProgressWnd(&wndProgress);
    
    CString first,last, every;
    first.Format("%02d:%02d:00", atoi(m_FirstHH), atoi(m_FirstMM));
    last.Format("%02d:%02d:00", atoi(m_LastHH), atoi(m_LastMM));
	every.Format("%02d:%02d", atoi(m_FreqMM), atoi(m_FreqSS));
    
	try
		{
		if (m_FileName.Right(4).CompareNoCase(".wav"))  // its not a wave file
			{
			if (theApp.m_Recs.MakeRecording( first, last, every, m_FileName.Left(m_FileName.GetLength() -3) + "wav"))
				theApp.m_Recs.ConvertToMp3(m_FileName);
			wndProgress.SetRange(0,400);
			}
		else  // just wave file
			{
			theApp.m_Recs.MakeRecording( first, last, every, m_FileName);
			wndProgress.SetRange(0,300);
			}
		}
	catch (CString s)
		{
		theApp.m_Recs.ClearProgressWnd();
		AfxMessageBox(CString("Unexpected error: " + s), MB_ICONEXCLAMATION);
		}
    theApp.m_Recs.ClearProgressWnd();
    
    this->EnableWindow(true);

    m_InCreate = false;
    EnableCtrls();
	}

void CCountdownDlg::OnButtonRecord() 
    {
       
	if(m_Recording)
	    {
        CString name;
		m_NewMsgCtrl.GetWindowText(name);
		m_RecordingCtrl.SetWindowText("&Start Recording");
		m_RecordingCtrl.EnableWindow(FALSE);
		m_RecordThread->PostThreadMessage(WM_RECORDSOUND_STOPRECORDING, 0, 0L);

        PopulateCombos();
		m_NewMsgCtrl.SelectString(-1,name);
		// Rest of interface fix-up is done in SoundFileClosed
    	}
	else
	    {
        CString name;
        m_NewMsgCtrl.GetWindowText(name);
        if (name.IsEmpty())
            {
            AfxMessageBox("Name for a recording must be supplied.", MB_OK|MB_APPLMODAL|MB_ICONEXCLAMATION);
            this->m_NewMsgCtrl.SetFocus();
            return;
            } 
        if (name.Left(6) == "cdhour" ||
            name.Left(5) == "cdnum")
            {
            AfxMessageBox("Names starting 'cdhour' or 'cdnum' are reserved.", MB_OK|MB_APPLMODAL|MB_ICONEXCLAMATION);
            this->m_NewMsgCtrl.SetFocus();
            return;
            }

        m_LedCtrl.SetOnOff(true);
        //m_LedCtrl.SetBlink(500);
		m_RecordingCtrl.SetWindowText("&Stop Recording");
        this->m_NewMsgCtrl.EnableWindow(false);

       
        //Build whole path
        name.MakeLower();
        if (name.GetLength() < 4 || name.Right(4) != ".wav")
            name += ".wav";

        CString* FileName = new CString();
        FileName->Format("%s\\%s", theApp.CustomSoundDir(), name);
		theApp.SetNotifyHwnd(GetSafeHwnd());
		m_RecordThread->PostThreadMessage(WM_RECORDSOUND_STARTRECORDING, 0, (LPARAM)FileName);

		m_Recording = TRUE;
	    }

    }


void CCountdownDlg::PopulateCombos()
    {
    bool result (true);
    CFileFind finder;
	CString saveContent, saveFreqMsg, saveNewMsg;

	if (m_ContentMsgCtrl.GetCurSel() >= 0)
		m_ContentMsgCtrl.GetLBText(m_ContentMsgCtrl.GetCurSel(),saveContent);
	if (m_FreqMsgCtrl.GetCurSel() >= 0)
		m_FreqMsgCtrl.GetLBText(m_FreqMsgCtrl.GetCurSel(),saveFreqMsg);
	if (m_NewMsgCtrl.GetCurSel() >= 0)
		m_NewMsgCtrl.GetLBText(m_NewMsgCtrl.GetCurSel(),saveNewMsg);

	m_ContentMsgCtrl.ResetContent();
	m_FreqMsgCtrl.ResetContent();
	m_NewMsgCtrl.ResetContent();
	m_ContentMsgCtrl.AddString("<Next Minute>");
	m_ContentMsgCtrl.AddString("<Next Minute + 1>");
	m_ContentMsgCtrl.AddString("<Next Minute + 2>");
	m_ContentMsgCtrl.AddString("<Next Minute + 3>");
	m_ContentMsgCtrl.AddString("<Next Minute + 4>");
	m_ContentMsgCtrl.AddString("<Starters + 1>");
	m_ContentMsgCtrl.AddString("<Starters + 2>");
	m_ContentMsgCtrl.AddString("<Starters + 3>");
	m_ContentMsgCtrl.AddString("<Starters + 4>");
	m_ContentMsgCtrl.AddString("<Starters + 5>");


    result = !!finder.FindFile(theApp.CustomSoundDir() + "\\*.wav");

    while (result)
        {
        result = !!finder.FindNextFile();
        CString name = finder.GetFileName().Left(finder.GetFileName().GetLength() - 4);
        if (name.Left(6) != "cdhour" && 
            name.Left(5) != "cdnum")
            {
			if (m_ContentMsgCtrl.FindStringExact(0, name) < 0 &&
				!theApp.m_Recs.RecordExists(name))
				{
				m_ContentMsgCtrl.AddString(name);
	            if (m_ContentMsgCtrl.GetWindowTextLength() == 0)
	                m_ContentMsgCtrl.SelectString(0, name);
				}
			m_NewMsgCtrl.AddString(name);
            }
        }

	for (int i = 0; i < theApp.m_Recs.Count(); i++)
		{
		CRecording* rec = theApp.m_Recs.GetRec(i);
		if (m_FreqMsgCtrl.FindStringExact(0, rec->Name()) < 0 &&
			!theApp.m_Recs.HasDependent(rec->Name()))
			m_FreqMsgCtrl.AddString(rec->Name());
		}

	if (m_ModifyMode)
		{
		CRecording* rec = theApp.m_Recs.GetRec(m_ListMsgs.GetItemText(m_ListMsgs.GetSelectionMark(),0));
		if (rec->IsAfterMsg())
			m_FreqMsgCtrl.AddString(rec->FollowMsgName());
		if (!rec->IsGenerated())
			m_ContentMsgCtrl.AddString(rec->Name());
		}				

	if (!saveContent.IsEmpty() && m_ContentMsgCtrl.FindStringExact(0,saveContent) >= 0)
		m_ContentMsgCtrl.SetCurSel(m_ContentMsgCtrl.FindStringExact(0,saveContent));
	if (!saveFreqMsg.IsEmpty() && m_FreqMsgCtrl.FindStringExact(0,saveFreqMsg) >= 0)
		m_FreqMsgCtrl.SetCurSel(m_FreqMsgCtrl.FindStringExact(0,saveFreqMsg));
	if (!saveNewMsg.IsEmpty() && m_NewMsgCtrl.FindStringExact(0,saveNewMsg) >= 0)
		m_NewMsgCtrl.SetCurSel(m_NewMsgCtrl.FindStringExact(0,saveNewMsg));

    }

void CCountdownDlg::EnableCtrls()
    {
    GetDlgItem(IDC_RADIO_FREQ_AFTER_MSG)->EnableWindow(m_ListMsgs.GetItemCount() > 0);
    if (m_ListMsgs.GetItemCount() == 0)
        m_FreqRadioCtrl.SetCheck(1);

    m_FreqMsgCtrl.EnableWindow(!IsChecked(IDC_RADIO_FREQ_EVERY));
    if (!IsChecked(IDC_RADIO_FREQ_EVERY))
        {
        CString temp;
        m_FreqMsgCtrl.GetWindowText(temp);
        m_AddBtnCtrl.EnableWindow(!temp.IsEmpty());
        m_RelativeCtrl.SetCurSel(0);
        }
    else
        {
        m_AddBtnCtrl.EnableWindow(TRUE);            
        }

    m_ContentMsgCtrl.EnableWindow(IsChecked(IDC_RADIO_CONTENT_MSG) && !m_ModifyMode);
	m_NextPlusMMCtrl.EnableWindow(IsChecked(IDC_RADIO_CONTENT_TIME));
	m_StartersPlusMMCtrl.EnableWindow(IsChecked(IDC_RADIO_CONTENT_STARTERS));
	m_ContentRadioCtrl.EnableWindow(!m_ModifyMode);

	static_cast<CButton*>(GetDlgItem(IDC_RADIO_CONTENT_TIME))->EnableWindow(!m_ModifyMode);

    m_RelativeCtrl.EnableWindow(IsChecked(IDC_RADIO_FREQ_EVERY));
	m_TimingMMCtrl.EnableWindow(IsChecked(IDC_RADIO_FREQ_EVERY));
	m_TimingSSCtrl.EnableWindow(IsChecked(IDC_RADIO_FREQ_EVERY));
	m_TimingTTCtrl.EnableWindow(IsChecked(IDC_RADIO_FREQ_EVERY));

    CString temp;
    m_FileNameCtrl.GetWindowText(temp);
    m_CreateCtrl.EnableWindow(!temp.IsEmpty());
    m_PlayRecCtrl.EnableWindow(!temp.IsEmpty() && FileExists(temp));
    m_Menu.EnableMenuItem(ID_MENU_CREATE, temp.IsEmpty() ? MF_DISABLED | MF_GRAYED : MF_ENABLED);

    if (m_NewMsgCtrl.GetCurSel() > -1)
        m_NewMsgCtrl.GetLBText(m_NewMsgCtrl.GetCurSel(), temp);
    else
        m_NewMsgCtrl.GetWindowText(temp);
    m_RecordingCtrl.EnableWindow(!temp.IsEmpty());
    m_AmplifyCtrl.EnableWindow(!temp.IsEmpty());
	m_DampenCtrl.EnableWindow(!temp.IsEmpty());
    m_PlayCtrl.EnableWindow(!temp.IsEmpty());

    m_DeleteCtrl.EnableWindow(m_ListMsgs.GetSelectedCount() > 0 && !m_ModifyMode); 
	m_ModifyCtrl.EnableWindow(m_ListMsgs.GetSelectedCount() == 1  && !m_ModifyMode); 
	m_AddAfterCtrl.EnableWindow(m_ListMsgs.GetSelectedCount() == 1 && !m_ModifyMode &&
		!theApp.m_Recs.HasDependent(m_ListMsgs.GetItemText(m_ListMsgs.GetSelectionMark(),0))); 
	m_ListMsgs.EnableWindow(!m_ModifyMode);
	m_CancelBtnCtrl.EnableWindow(m_ModifyMode);
	
	if (m_ModifyMode)
		m_AddBtnCtrl.SetWindowText("Update");
	else
		m_AddBtnCtrl.SetWindowText("Add");
    }

bool CCountdownDlg::IsChecked(int a_ID)
    {
    int i = this->GetCheckedRadioButton(a_ID, a_ID + 1);
    return i == a_ID;
    }


void CCountdownDlg::OnRadioContentMsg() 
	{
	EnableCtrls();
	
	}

void CCountdownDlg::OnRadioContentTime() 
	{
	EnableCtrls();
	
	}

void CCountdownDlg::OnRadioFreqAfterMsg() 
	{
    if (IsChecked(IDC_RADIO_FREQ_AFTER_MSG))
        m_FreqRadioCtrl.SetCheck(0);
	EnableCtrls();
	
	}

void CCountdownDlg::OnRadioFreqEvery() 
	{
    if (IsChecked(IDC_RADIO_FREQ_EVERY))
        static_cast<CButton*>(GetDlgItem(IDC_RADIO_FREQ_AFTER_MSG))->SetCheck(0);
	EnableCtrls();	
	}

void CCountdownDlg::OnRadioTimingFinish() 
	{
	EnableCtrls();
	
	}

void CCountdownDlg::OnRadioTimingStart() 
	{
	EnableCtrls();	
	}

void CCountdownDlg::ModifyRec()
	{
	// change of message and change of type of message not allowed (prevented by controls)
	// should be able to do anything else (??)
	CString beforeAfter;
    m_RelativeCtrl.GetWindowText(beforeAfter);
	
	CString timing;
    timing.Format("%s:%s.%s", m_TimingMM, m_TimingSS, m_TimingTT);
    timing.Replace(' ', '0');

	theApp.m_Recs.ModifyRecord(m_ListMsgs.GetItemText(m_ListMsgs.GetSelectionMark(),0), //name
                               IsChecked(IDC_RADIO_CONTENT_MSG),   //is custom message
							   !IsChecked(IDC_RADIO_FREQ_EVERY),    //has own time anchor
							   timing,
							   beforeAfter == "Finishing",         //finishes at specified time
							   m_FreqMsg,                          // rec that record follows
							   m_NextPlusMM);                       //offset
	m_ModifyMode = false;
	PopulateCombos();
	RedrawList();
	EnableCtrls();

	}

void CCountdownDlg::OnButtonAdd() 
    {
    m_InAdd = true;
	if (!UpdateData(TRUE)) 
        {
        m_InAdd = false;
        return;
        }

    m_InAdd = false;

	
	if (m_ModifyMode)
		{
		ModifyRec();
		return;
		}


    CString timing;

    timing.Format("%s:%s.%s", m_TimingMM, m_TimingSS, m_TimingTT);
    timing.Replace(' ', '0');
   
    CRecording* newRec;
    CString beforeAfter;
    m_RelativeCtrl.GetWindowText(beforeAfter);

    if (IsChecked(IDC_RADIO_CONTENT_MSG) && m_ContentMsg.Left(1) != "<") // Its a message format
        if (IsChecked(IDC_RADIO_FREQ_EVERY))
            newRec = theApp.m_Recs.AddCustomRecord( m_ContentMsg,
                                                beforeAfter == "Finishing",
                                                timing);
        else
            newRec = theApp.m_Recs.AddCustomRecord(m_ContentMsg,
                                                m_FreqMsg);
	else
		{                               // Its a time like <Next Minute> or a start list like <Starters + 4>
		bool isStartList = (IsChecked(IDC_RADIO_CONTENT_MSG) && m_ContentMsg.Left(6) == "<Start") || IsChecked(IDC_RADIO_CONTENT_STARTERS);
		CString nextPlus("00");
		if (IsChecked(IDC_RADIO_CONTENT_MSG))
			{
			for (int i = 0; i < m_ContentMsg.GetLength(); i++)
				if (isdigit(m_ContentMsg[i]))
					{
					nextPlus = "0" + m_ContentMsg[i];
					break;
					}
			}
		else 
			nextPlus = isStartList ? m_StartersPlusMM : m_NextPlusMM;		
		
		if (isStartList)
			{
			if (IsChecked(IDC_RADIO_FREQ_EVERY))
				newRec = theApp.m_Recs.AddStartListRecord(beforeAfter == "Finishing", timing, nextPlus);
			else
				newRec = theApp.m_Recs.AddStartListRecord(m_FreqMsg, nextPlus);
			}
		else
			{
			if (IsChecked(IDC_RADIO_FREQ_EVERY))
				newRec = theApp.m_Recs.AddTimeRecord(beforeAfter == "Finishing", timing, nextPlus);
			else
				newRec = theApp.m_Recs.AddTimeRecord(m_FreqMsg, nextPlus);
			}

		}

    if (!newRec)
        AfxMessageBox("Couldn't add new recording: " + theApp.m_Recs.LastError(), MB_OK|MB_APPLMODAL|MB_ICONEXCLAMATION);
    else
        {
        RedrawList();
        EnableCtrls();
		PopulateCombos();
        }
    }

CString CCountdownDlg::TimeString(const CString& a_HH, const CString& a_MM, const CString& a_SS, CString a_TT) const
    {
    return CString(a_HH + ":" + a_MM + ":" + a_SS + "." + a_TT);
    }

void CCountdownDlg::RedrawList()
	{
	m_ListMsgs.DeleteAllItems();
    CRecording** array = theApp.m_Recs.GetSortedRecords();

    for (int i = 0; i < theApp.m_Recs.Count(); i++)
        {
        AddListEntry(array[i]);
        }
    delete [] array;
	}

void CCountdownDlg::AddListEntry(CRecording* a_Rec)
    {
	m_InterimChange = true;
	UpdateData(true);
	m_InterimChange = false;
    int index;

    // Find insert position 
    if (a_Rec->IsAfterMsg())
        {
        for (index = 0; index < m_ListMsgs.GetItemCount(); index++)
            {
            if (m_ListMsgs.GetItemText(index, 0) == a_Rec->FollowMsgName())
                {
                index++;
                break;
                }
            }
        }
    else
        {
        for (index = 0; index < m_ListMsgs.GetItemCount(); index++)
            {
            CRecording* rec = reinterpret_cast<CRecording*> (m_ListMsgs.GetItemData(index));
            if (!rec->IsAfterMsg())
				{
				double offset1(SpanToFloatSecs(HHMMSST2Span(a_Rec->TimingTime()))),
					   offset2(SpanToFloatSecs(HHMMSST2Span(rec->TimingTime())));
				if (a_Rec->FinishAtTime())
					offset1 -= a_Rec->MaxDuration();
				if (rec->FinishAtTime())
					offset2 -= rec->MaxDuration();
                if (offset1 < offset2)
                    {
                    break;
                    }
				}
			else if (rec->FollowMsgName() == a_Rec->Name())
				{
				break;
				}
            }
        }
    
    index = m_ListMsgs.InsertItem(index, a_Rec->Name(), a_Rec->ImageIndex());
    CString temp;
    temp.Format("%03.2f", a_Rec->MaxDuration());
    m_ListMsgs.SetItemText(index, 1, temp);

// Item 2 is "SPECIAL:" then a comma-separated list,the period interval,recording maxduration, offset, all in seconds
	float offset(0);
	CRecording* rec;
	rec = a_Rec;
	while(rec->IsAfterMsg())
		{	
		if (theApp.m_Recs.RecordExists(rec->FollowMsgName()))
			{
			rec=theApp.m_Recs.GetRec(rec->FollowMsgName());
			offset += rec->MaxDuration();
			}
		}
	ASSERT(!rec->IsAfterMsg());
	CString temp1(rec->TimingTime());
	offset += SpanToFloatSecs(HHMMSST2Span(temp1));
    if (rec->FinishAtTime())
		offset -= rec->MaxDuration();

	temp.Format("SPECIAL:%d,%f,%f,%d", atoi(this->m_FreqMM)*60+atoi(this->m_FreqSS),a_Rec->MaxDuration(),offset,a_Rec->IsAfterMsg());
    m_ListMsgs.SetItemText(index, 2, temp);
    m_ListMsgs.SetItemData(index, (long)a_Rec);
    }


void CCountdownDlg::OnSelchangeComboFreqMsg() 
    {
	EnableCtrls();	
    }

void CCountdownDlg::OnButtonDelete() 
	{

	auto count (m_ListMsgs.GetSelectedCount() + 1);
    while (m_ListMsgs.GetSelectedCount() < count)
        {
      	POSITION pos = m_ListMsgs.GetFirstSelectedItemPosition();
    	if (pos == NULL)
	    	return;

        count = m_ListMsgs.GetSelectedCount();
        while (pos)
		    {
            //pos = m_ListMsgs.GetFirstSelectedItemPosition();
		    int nItem = m_ListMsgs.GetNextSelectedItem(pos);
		    theApp.m_Recs.DeleteRecord(m_ListMsgs.GetItemText(nItem,0));
		    }	
        }

	PopulateCombos();
	RedrawList();

    // Only messages left are the ones that have dependencies to them that aren't being deleted
   	POSITION pos = m_ListMsgs.GetFirstSelectedItemPosition();
   	if (pos == NULL)
    	return;
    CString names;

    while (pos)
		{
        //pos = m_ListMsgs.GetFirstSelectedItemPosition();
		int nItem = m_ListMsgs.GetNextSelectedItem(pos);
        CString name = m_ListMsgs.GetItemText(nItem,0);
        if (names.IsEmpty())
            names = name;
        else
            names += "," + name;
		}	

    AfxMessageBox("Couldn't delete items : " + names + ". Some recording runs after them.", MB_OK|MB_APPLMODAL|MB_ICONEXCLAMATION);
	}


void CCountdownDlg::OnButtonTest() 
    {   
    // Get the amplitudes for all files in the default sound directory
	    // Find out where file is
    char szAppPath[MAX_PATH] = "";
    CString strAppName, defaultDir;

    ::GetModuleFileName(0, szAppPath, MAX_PATH);

    strAppName = szAppPath;
    strAppName = strAppName.Left(strAppName.ReverseFind('\\'));

    defaultDir.Format("%s\\sounds", strAppName);
    
    bool result;
    CFileFind finder;
    result = !!finder.FindFile(defaultDir + "\\*.wav");
    
    short maxPeak(0);
    short maxAvgPeak(0);
    CString maxPeakName, maxAvgPeakName;

    bool doneCustom(false);
    int count(0);
    double sumAvgPeak(0);
restart:
    while (result)
        {
        result = !!finder.FindNextFile();
        CString name = finder.GetFilePath();
        CWave wave(name);
        short peak, avgpeak;
        wave.getAmplitude(peak, avgpeak);        
        sumAvgPeak += avgpeak;
        ++count;

        if (peak > maxPeak)
            {
            maxPeak = peak;
            maxPeakName = name;
            }

        if (avgpeak > maxAvgPeak)
            {
            maxAvgPeak = avgpeak;
            maxAvgPeakName = name;
            }
        //TRACE3("Peak data for %s - peak: %d, smoothed peak: %d\n", name, peak, avgpeak);
        }
    TRACE2("Max peak found in %s - peak: %d\n", maxPeakName, maxPeak);
    TRACE2("Max smoothed peak found in %s - smoothed peak: %d\n", maxAvgPeakName, maxAvgPeak);
    TRACE1("The average smoothed peak was %d\n", (int)(sumAvgPeak/count));

    if (!doneCustom)
        {
        doneCustom = true;
        maxPeak = 0;
        maxAvgPeak = 0;
        sumAvgPeak = 0;
        count = 0;
        result = !!finder.FindFile(theApp.CustomSoundDir() + "\\*.wav");
        goto restart;
        }
    }


void CCountdownDlg::OnButtonBrowse() 
    {
    m_FileNameCtrl.GetWindowText(m_FileName);
    CFileDialog l_SampleDlg(FALSE,".mp3", m_FileName,OFN_OVERWRITEPROMPT,"MP3 Files (*.mp3)|*.mp3|Wave files(*.wav)|*.wav||");
    int iRet = l_SampleDlg.DoModal();
    CString l_strFileName;
    l_strFileName = l_SampleDlg.GetPathName();

    if(iRet == IDOK)
        {
        m_FileNameCtrl.SetWindowText(l_strFileName);
        m_FileNameCtrl.GetWindowText(m_FileName);
        }    
    }

void CCountdownDlg::OnChangeEditFilename() 
    {
    m_FileNameCtrl.GetWindowText(m_FileName);
    EnableCtrls();
    }

void CCountdownDlg::OnEditchangeComboRecordMsg() 
    {
	EnableCtrls();
    }   

void CCountdownDlg::OnClickListMessages(NMHDR* pNMHDR, LRESULT* pResult) 
    {
	EnableCtrls();	
	*pResult = 0;
    }

void CCountdownDlg::OnDeleteitemListMessages(NMHDR* pNMHDR, LRESULT* pResult) 
    {
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	EnableCtrls();	
	*pResult = 0;
    }

void CCountdownDlg::OnSetfocusListMessages(NMHDR* pNMHDR, LRESULT* pResult) 
    {
	EnableCtrls();		
	*pResult = 0;
    }

void CCountdownDlg::OnButtonAmplify() 
    {
    ModifyVolume(20);
	}

void CCountdownDlg::ModifyVolume(int a_PctChange) 
	{
    //Build whole path
	CString temp;
    m_NewMsgCtrl.GetWindowText(temp);
    if (temp.IsEmpty())
        {
        AfxMessageBox("No recording selected", MB_OK|MB_APPLMODAL|MB_ICONEXCLAMATION);
        return;
        }


    temp.MakeLower();
    if (temp.GetLength() < 4 || temp.Right(4) != ".wav")
        temp += ".wav";

    CString Filetemp;
    Filetemp.Format("%s\\%s", theApp.CustomSoundDir(), temp);

    if (!FileExists(Filetemp))
        {
        AfxMessageBox("Message " + temp + ": Recording doesn't exist", MB_OK|MB_APPLMODAL|MB_ICONEXCLAMATION);
        return;
        }


    CWave wave(Filetemp);
    if (!wave.IsValid())
        {
        AfxMessageBox(wave.LastError(), MB_ICONEXCLAMATION);
		return;
        }
    float boost = (float)(100 + a_PctChange);
    boost /= 100;
    wave.Normalise(boost);
    wave.saveToFile();
    AfxMessageBox("Volume modified", MB_OK|MB_APPLMODAL|MB_ICONINFORMATION);
    }

void CCountdownDlg::OnSelchangeComboRecordMsg() 
    {
	EnableCtrls();
    }

void CCountdownDlg::OnButtonPlay() 
    {
	CString FileName, temp;
    m_NewMsgCtrl.GetWindowText(temp);
    FileName.Format("%s\\%s.wav", theApp.CustomSoundDir(), temp);
    if (FileExists(FileName))
        {
         //::ShellExecute(AfxGetMainWnd()->m_hWnd, "open", FileName, NULL,NULL, SW_SHOWNORMAL);
		PlaySound(FileName, NULL, SND_FILENAME);
        }
    }


void CCountdownDlg::OnSelchangeComboRelative() 
{
	// TODO: Add your control notification handler code here
	
}

void CCountdownDlg::OnChangeEditFreqHh() 
    {
	EnableCtrls();
    }

void CCountdownDlg::OnChangeEditFreqMm() 
    {
	EnableCtrls();
	RedrawList();
    }

void CCountdownDlg::OnChangeEditFreqSs() 
    {
	EnableCtrls();
	RedrawList();
    }

void CCountdownDlg::OnMenuAbout() 
    {
	CAboutDlg dlg;
    dlg.DoModal();	
    }

void CCountdownDlg::OnMenuClock() 
    {
	CRecordTimesDlg dlg(this, m_RecordThread);
    dlg.DoModal();
    }

void CCountdownDlg::OnMenuCreate() 
    {
	this->OnButtonCreate();
    }

void CCountdownDlg::OnMenuExit() 
    {
    m_InExit = true;
	UpdateData(TRUE);
	theApp.m_Recs.StartTime(m_FirstHH + ':' + m_FirstMM);
	theApp.m_Recs.StopTime(m_LastHH + ':' + m_LastMM);
	theApp.m_Recs.Frequency(m_FreqMM + ':' + m_FreqSS);
    
    CString name;
    m_FileNameCtrl.GetWindowText(name);
    theApp.m_Recs.SetFileName(name);

	OnCancel();
    }

void CCountdownDlg::OnMenuHelpContents() 
    {
	OnHelpFinder();	
    }

void CCountdownDlg::OnMenuMaintenance() 
    {
	CMaintenanceDlg dlg(this);
    dlg.DoModal();	
    PopulateCombos();
    EnableCtrls();
    }

BOOL CCountdownDlg::PreTranslateMessage(MSG* pMsg)
    {
    // TODO: Add your specialized code here and/or call the base class

    // Magical mystical MFC F1 Help msg!
    if(pMsg->message == 0x4d) 
    {
        if (GetKeyState(VK_SHIFT) >= 0) 
        {
            // Shift key not down

            // Supply general dialog level help
            OnCommandHelp(0, 10);

            return(TRUE); // Eat it
        }
#ifdef FLOATING_ARROW
        else 
        {
            // Use this if you want Shift+F1
            // to create the floating arrow instead
            SendMessage(WM_SYSCOMMAND, SC_CONTEXTHELP);

            return(TRUE);
        }
#endif // FLOATING_ARROW
    }

    return CDialog::PreTranslateMessage(pMsg);
    }


void CCountdownDlg::OnClose() 
    {
	m_InExit = true;
	UpdateData(TRUE);
	theApp.m_Recs.StartTime(m_FirstHH + ':' + m_FirstMM);
	theApp.m_Recs.StopTime(m_LastHH + ':' + m_LastMM);
    
    CString name;
    m_FileNameCtrl.GetWindowText(name);
    theApp.m_Recs.SetFileName(name);
	m_StartListCtrl.GetWindowText(m_StartListFile);
    theApp.m_Recs.SetStartListFile(m_StartListFile);
	
	CDialog::OnClose();
    }


void CCountdownDlg::OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu)
{
    ASSERT(pPopupMenu != NULL);
    // Check the enabled state of various menu items.

    CCmdUI state;
    state.m_pMenu = pPopupMenu;
    ASSERT(state.m_pOther == NULL);
    ASSERT(state.m_pParentMenu == NULL);

    // Determine if menu is popup in top-level menu and set m_pOther to
    // it if so (m_pParentMenu == NULL indicates that it is secondary popup).
    HMENU hParentMenu;
    if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
        state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
    else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
    {
        CWnd* pParent = this;
           // Child windows don't have menus--need to go to the top!
        if (pParent != NULL &&
           (hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
        {
           int nIndexMax = ::GetMenuItemCount(hParentMenu);
           for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
           {
            if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
            {
                // When popup is found, m_pParentMenu is containing menu.
                state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
                break;
            }
           }
        }
    }

    state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
    for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
      state.m_nIndex++)
    {
        state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
        if (state.m_nID == 0)
           continue; // Menu separator or invalid cmd - ignore it.

        ASSERT(state.m_pOther == NULL);
        ASSERT(state.m_pMenu != NULL);
        if (state.m_nID == (UINT)-1)
        {
           // Possibly a popup menu, route to first item of that popup.
           state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
           if (state.m_pSubMenu == NULL ||
            (state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
            state.m_nID == (UINT)-1)
           {
            continue;       // First item of popup can't be routed to.
           }
           state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
        }
        else
        {
           // Normal menu item.
           // Auto enable/disable if frame window has m_bAutoMenuEnable
           // set and command is _not_ a system command.
           state.m_pSubMenu = NULL;
           state.DoUpdate(this, FALSE);
        }

        // Adjust for menu deletions and additions.
        UINT nCount = pPopupMenu->GetMenuItemCount();
        if (nCount < state.m_nIndexMax)
        {
           state.m_nIndex -= (state.m_nIndexMax - nCount);
           while (state.m_nIndex < nCount &&
            pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
           {
            state.m_nIndex++;
           }
        }
        state.m_nIndexMax = nCount;
    }
}
					

void CCountdownDlg::OnButtonPlayrec() 
    {
	CString FileName, temp;
    m_FileNameCtrl.GetWindowText(temp);
    if (FileExists(temp))
        {
         ::ShellExecute(AfxGetMainWnd()->m_hWnd, "open", temp, NULL,NULL, SW_SHOWNORMAL);
        }
	
    }

void CCountdownDlg::OnButtonAddafter() 
	{
	// Should just be one selected item in list
	m_FreqMsg = m_ListMsgs.GetItemText(m_ListMsgs.GetSelectionMark(),0);
	m_FreqRadio = 1;
	UpdateData(false);
	EnableCtrls();

	}

void CCountdownDlg::OnButtonDampen() 
	{
	ModifyVolume(-15);
	}

void CCountdownDlg::OnButtonModify() 
	{
	m_ModifyMode = true;
	CRecording* rec = theApp.m_Recs.GetRec(m_ListMsgs.GetItemText(m_ListMsgs.GetSelectionMark(),0));
	if (!rec)
		{
		m_ModifyMode = false;
		return;
		}
	else
		{
		m_ModifyMode = true;
		PopulateCombos();

		m_ContentRadioCtrl.SetCheck(rec->IsGenerated() ? 0 : 1);
		static_cast<CButton*>(GetDlgItem(IDC_RADIO_CONTENT_TIME))->SetCheck(rec->IsGenerated() ? 1 : 0);

		if (rec->IsGenerated())
			{
			CTimeRecording *rec = static_cast<CTimeRecording*>(theApp.m_Recs.GetRec(m_ListMsgs.GetItemText(m_ListMsgs.GetSelectionMark(),0)));
			m_NextPlusMMCtrl.SetWindowText(rec->OffsetTime());
			}
		else
			{
			CCustomRecording *rec = static_cast<CCustomRecording*>(theApp.m_Recs.GetRec(m_ListMsgs.GetItemText(m_ListMsgs.GetSelectionMark(),0)));
			m_ContentMsgCtrl.SelectString(-1, rec->Name());
			}

		m_FreqRadioCtrl.SetCheck(rec->IsAfterMsg() ? 0 : 1);
		static_cast<CButton*>(GetDlgItem(IDC_RADIO_FREQ_AFTER_MSG))->SetCheck(rec->IsAfterMsg() ? 1 : 0);
		if (rec->IsAfterMsg())
			{
			m_FreqMsgCtrl.SelectString(-1, rec->FollowMsgName());
			}
		else
			{
			CString timing = rec->TimingTime();
			m_TimingMMCtrl.SetWindowText(Word(timing, 0, ':'));
			m_TimingSSCtrl.SetWindowText(Word(Word(timing, 1, ':'), 0, '.'));
			m_TimingTTCtrl.SetWindowText(Word(timing, 1, '.'));
			if (rec->FinishAtTime())
				m_RelativeCtrl.SelectString(1, "Finishing");
			else
				m_RelativeCtrl.SelectString(-1, "Starting");
			}

		//UpdateData(false);
		EnableCtrls();
		}

	}

bool CCountdownDlg::CopyAllUsedSounds(CFileOperation& fo, CString folder)
	{
	CString customDir = theApp.CustomSoundDir();
    CString defaultDir, strAppName;
	char szAppPath[MAX_PATH] = "";

    ::GetModuleFileName(0, szAppPath, MAX_PATH);

	strAppName = szAppPath;
    strAppName = strAppName.Left(strAppName.ReverseFind('\\'));
    defaultDir.Format("%s\\sounds", strAppName);

	CFileFind finder;
    bool result;

	// Copy all the time messages across
    result = !!finder.FindFile(defaultDir + "\\*.wav");
    while (result)
        {
		CFileStatus status;
        result = !!finder.FindNextFile();
        CString fname = finder.GetFilePath();
        CString name = finder.GetFileName().Left(finder.GetFileName().GetLength());
		if (CFile::GetStatus(customDir + CString("\\") + name, status))
			{
			if (!fo.Copy(customDir + "\\" + name, folder)) // do Copy
				{
				fo.ShowError(); // if copy fails show error message
				return false;
				}
			}
		else
			{
			if (!fo.Copy(defaultDir + "\\" + name, folder)) // do Copy
				{
				fo.ShowError(); // if copy fails show error message
				return false;
				}
			}
		}

	// Copy all custom messages used in the recording across

    result = !!finder.FindFile(customDir + "\\*.wav");

    while (result)
        {
        result = !!finder.FindNextFile();
        CString fname = finder.GetFilePath();
        CString name = finder.GetFileName().Left(finder.GetFileName().GetLength());
        if (name.Left(6) != "cdhour" && 
            name.Left(5) != "cdnum" &&
            theApp.m_Recs.RecordExists(name.Left(name.GetLength()-4)))
            {
			if (!fo.Copy(customDir + "\\" + name, folder)) // do Copy
				{
				fo.ShowError(); // if copy fails show error message
				return false;
				}
            }
        }
	
	return true;
	}


void CCountdownDlg::OnMenuExport() 
	{
	UpdateData(FALSE);
	//Get a *new* folder location
	CFileDialog dlg(FALSE,NULL, m_FileName,OFN_OVERWRITEPROMPT,"Folders | ||");
	dlg.m_ofn.lpstrTitle = "Export to new folder";
	int result = dlg.DoModal();
	CString file = dlg.GetFileName();
	CString folder = dlg.GetPathName();

	if (result == IDOK)
		{
		CFileOperation fo;      // create object
		fo.SetOverwriteMode(false); // reset OverwriteMode flag (optional)
		MakeSureDirectoryPathExists(folder+"\\Custom\\dummy.txt");
		
		if (!CopyAllUsedSounds(fo, folder + CString("\\Custom")))
			return;

		theApp.m_Recs.SaveToXML(folder + "\\" + theApp.ConfigFile().Mid(theApp.ConfigFile().ReverseFind('\\')+1));
		//CopyFile(theApp.ConfigFile(),folder + "\\" + theApp.ConfigFile().Mid(theApp.ConfigFile().ReverseFind('\\')+1),false);
		}
	}

void CCountdownDlg::OnMenuImport() 
	{
	char buf[MAX_PATH];
	
	// Get an existing folder 
	if (XBrowseForFolder(this->GetSafeHwnd(),(LPCTSTR)CSIDL_DESKTOP,-1,"Import Folder",buf,MAX_PATH))
		{
		// Check that folder has the right structure
		CString folder(buf);
		CString config = theApp.ConfigFile().Mid(theApp.ConfigFile().ReverseFind('\\')+1);
		CString soundDir = theApp.CustomSoundDir().Mid(theApp.CustomSoundDir().ReverseFind('\\')+1);
		if (!FileExists(folder + "\\" + config))
			{
			MessageBox("File " + folder + "\\" + config + " doesn't exist. Nothing imported.","Import Error",MB_OK|MB_ICONEXCLAMATION);
			return;
			}
		if (!FileExists(folder + "\\" + soundDir))
			{
			MessageBox("Folder " + folder + "\\" + soundDir + " doesn't exist. Nothing imported.","Import Error",MB_OK|MB_ICONEXCLAMATION);
			return;
			}

		// Copy the files
		CFileOperation fo;      // create object
		fo.SetOverwriteMode(true); // reset OverwriteMode flag (optional)
		MakeSureDirectoryPathExists(theApp.CustomDir()+"\\dummy.txt");
		if (!fo.Copy(folder + "\\" + soundDir, theApp.CustomDir())) // do Copy
			{
			fo.ShowError(); // if copy fails show error message
			return;
			}

		CopyFile(folder + "\\" + theApp.ConfigFile().Mid(theApp.ConfigFile().ReverseFind('\\')+1),theApp.ConfigFile(),false);
		theApp.Reload();
		
		InitialiseData();
		}
	
	}

void CCountdownDlg::OnCancelBtn() 
	{
	// TODO: Add your control notification handler code here
	m_ModifyMode = false;
	PopulateCombos();
	EnableCtrls();	
	}

LRESULT CCountdownDlg::OnClosedSoundFile(WPARAM wParam, LPARAM lParam)
	{
	if (m_Recording)
		{
		m_Recording = FALSE;
		m_LedCtrl.SetOnOff(false);
		m_NewMsgCtrl.EnableWindow(true);
		m_RecordingCtrl.EnableWindow(true);
		RedrawList();
		}
	return 0;
	}


void CCountdownDlg::OnMenuRecordingProperties()
{
	CRecordingPropsDlg dlg(this, theApp.m_Recs.GetVoice());
	if (dlg.DoModal() == IDOK)
		theApp.m_Recs.SetVoice(dlg.GetVoice());
}


void CCountdownDlg::OnBnClickedButtonBrowseStartlist()
{
	m_StartListCtrl.GetWindowText(m_StartListFile);
	CFileDialog l_SampleDlg(TRUE, ".xml", m_StartListFile, 0, "XML Files (*.xml)||");
	int iRet = l_SampleDlg.DoModal();
	CString l_strFileName;
	l_strFileName = l_SampleDlg.GetPathName();

	if (iRet == IDOK)
	{
		m_StartListCtrl.SetWindowText(l_strFileName);
		m_StartListCtrl.GetWindowText(m_StartListFile);
		theApp.m_Recs.SetStartListFile(m_StartListFile);
	}
}


void CCountdownDlg::OnEnChangeEditStartlistfile()
{
	//m_StartListCtrl.GetWindowText(m_StartListFile);
	//if (FileExists(m_StartListFile))
	//	theApp.m_Recs.SetStartListFile(m_StartListFile);
}


void CCountdownDlg::OnBnClickedRadioContentStarters()
{
	EnableCtrls();
}


void CCountdownDlg::OnBnClickedButtonGenerate()
{
	// Parse the start list to:
	// 1) Get a map of names for each time
	// 2) Make sure that we have the sound files for each time
	if (!FileExists(m_StartListFile))
		AfxMessageBox("Couldn't open file : " + m_StartListFile + ". It doesn't exist.", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
	theApp.StartList().Load(m_StartListFile);
}
