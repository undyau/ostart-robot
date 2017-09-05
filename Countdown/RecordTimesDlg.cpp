// RecordTimesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Countdown.h"
#include "RecordTimesDlg.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCountdownApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CRecordTimesDlg dialog


CRecordTimesDlg::CRecordTimesDlg(CWnd* pParent, CRecordSound* a_RecordThread)
	: CDialog(CRecordTimesDlg::IDD, pParent), m_RecordThread(a_RecordThread),
    m_Number(0), m_Recording(false)
{
	//{{AFX_DATA_INIT(CRecordTimesDlg)
	m_DisplayNumber = _T("");
	//}}AFX_DATA_INIT
}


void CRecordTimesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecordTimesDlg)
	DDX_Control(pDX, IDC_CHECK_SEQUENTIAL, m_SequentialCtrl);
	DDX_Control(pDX, IDC_BUTTON_RECORD, m_RecordingCtrl);
	DDX_Control(pDX, IDC_SPIN_NUMBER, m_SpinNumberCtrl);
	DDX_Control(pDX, IDC_DYN_LED, m_LedCtrl);
	DDX_Control(pDX, IDC_STATIC_NUMBER, m_DisplayNumberCtrl);
	DDX_Text(pDX, IDC_STATIC_NUMBER, m_DisplayNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecordTimesDlg, CDialog)
	//{{AFX_MSG_MAP(CRecordTimesDlg)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, OnButtonRecord)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_NUMBER, OnDeltaposSpinNumber)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WRITESOUNDFILE_CLOSEDFILE, OnClosedSoundFile)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordTimesDlg message handlers

void CRecordTimesDlg::OnButtonPlay() 
    {
    if (!m_Recording)
        {
        CString FileName;
        FileName.Format("%s\\%s", theApp.CustomSoundDir(), GetRecordingName(m_Number));
        if (FileExists(FileName))
           //::ShellExecute(AfxGetMainWnd()->m_hWnd, "open", FileName, NULL,NULL, SW_SHOWNORMAL);
			PlaySound(FileName, NULL, SND_FILENAME);
        }
    }

void CRecordTimesDlg::OnButtonRecord() 
    {
	if(m_Recording)
	    {       
		m_RecordingCtrl.SetWindowText("&Start Recording");
		theApp.SetNotifyHwnd(GetSafeHwnd());
		m_RecordThread->PostThreadMessage(WM_RECORDSOUND_STOPRECORDING, 0, 0L);
		m_RecordingCtrl.EnableWindow(FALSE);
	   	}
	else
	    {
        CString name = GetRecordingName(m_Number);
        //Build whole path
        name.MakeLower();
        CString* FileName = new CString();
        FileName->Format("%s\\%s", theApp.CustomSoundDir(), name);

		m_RecordThread->PostThreadMessage(WM_RECORDSOUND_STARTRECORDING, 0, (LPARAM)FileName);

        m_LedCtrl.SetOnOff(true);
        m_DisplayNumberCtrl.SetBkColor(RGB(255,0,0));
		m_RecordingCtrl.SetWindowText("&Stop Recording");       

		m_Recording = true;
	    }
    }

void CRecordTimesDlg::OnDeltaposSpinNumber(NMHDR* pNMHDR, LRESULT* pResult) 
    {
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    IncNumber(pNMUpDown->iDelta);
	
	*pResult = 1;  // set failure - we have managed the spin internally
    }

void CRecordTimesDlg::IncNumber(int a_Increment)
    {
    if (m_Number >= 92 && a_Increment > 0)
        {
        m_Number = 0;
        }
    else if (m_Number <= 0 && a_Increment < 0)
        {
        m_Number = 92;
        }
    else
       {
       m_Number += a_Increment;
       }

    m_SpinNumberCtrl.SetPos(m_Number);
    m_DisplayNumberCtrl.SetWindowText(NumberToText(m_Number));
    }

BOOL CRecordTimesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_LedCtrl.SetColor(RGB(255,0,0), RGB(100, 40, 40));
    m_LedCtrl.SetBlink(0);
    m_LedCtrl.SetOnOff(false);
    CRect rect;
    m_LedCtrl.GetWindowRect(rect);
    ScreenToClient(rect);
    rect.right = rect.left + 12;
    rect.bottom = rect.top + 12;
    m_LedCtrl.MoveWindow(rect);
	
    CFont font;
    if(font.CreatePointFont(600, "Arial"))
        { /* success */
          m_DisplayNumberCtrl.SetFont(&font);
          font.Detach( );
         } 

    m_DisplayNumberCtrl.SetWindowText(NumberToText(m_Number));
    m_SpinNumberCtrl.SetPos(m_Number);
    m_SpinNumberCtrl.SetRange(0, 92);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CRecordTimesDlg::NumberToText(int a_Number)
    {
    CString result;
    if (a_Number < 60) 
        {
        if (a_Number == 0)
            result = "0";
        else if (a_Number < 10)
            result.Format("%02d\n(0 prefix)", a_Number);
        else
            result.Format("%02d", a_Number);
        return result;
        }
    if (a_Number < 69) 
        {
        result.Format("%01d", a_Number - 59);
        return result;
        }

    if (a_Number < 93)
        {
        int hr = a_Number - 69;
        if (hr == 0)
            return "midnight\n(00:00)";
        if (hr == 12)
            return "noon\n(12:00)";
        if (hr < 12)
            {
            result.Format("%d o'clock\n(%d a.m.)", hr, hr);
            return result;
            }
        result.Format("%d o'clock\n(%d p.m.)", hr -12, hr -12);
        return result;
        }
    else
        {
        ASSERT (false);
        return "Ooops !";
        }
    }

CString CRecordTimesDlg::GetRecordingName(int a_Number)
    {
    CString result;
    if (a_Number < 60)
        result.Format("cdnum%02d.wav", a_Number); // 0,1,2,3..59
    else if (a_Number < 69)
        result.Format("cdnum%01d.wav", a_Number - 59);  // 1,2,3..9
    else
        result.Format("cdhour%02d.wav", a_Number-69);   // midnight,1am,.,11 pm
        
    return result;
    }

LRESULT CRecordTimesDlg::OnClosedSoundFile(WPARAM wParam, LPARAM lParam)
	{
	if (m_Recording)
		{
		m_RecordingCtrl.EnableWindow(TRUE);
		m_Recording = false;
        m_DisplayNumberCtrl.SetBkColor(GetSysColor(COLOR_BTNFACE));
        m_LedCtrl.SetOnOff(false);

        if (m_SequentialCtrl.GetCheck())
            IncNumber(1);
		}
	return 0;
	}