// MaintenanceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Countdown.h"
#include "MaintenanceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCountdownApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMaintenanceDlg dialog


CMaintenanceDlg::CMaintenanceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMaintenanceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMaintenanceDlg)
	m_RadioMessages = -1;
	//}}AFX_DATA_INIT
}


void CMaintenanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaintenanceDlg)
	DDX_Control(pDX, IDC_RADIO_MESSAGES, m_RadioCtrl);
	DDX_Control(pDX, ID_BTN_DELETE, m_DeleteCtrl);
	DDX_Control(pDX, IDC_LIST_RECS, m_ListRecsCtrl);
	DDX_Radio(pDX, IDC_RADIO_MESSAGES, m_RadioMessages);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMaintenanceDlg, CDialog)
	//{{AFX_MSG_MAP(CMaintenanceDlg)
	ON_BN_CLICKED(ID_BTN_DELETE, OnBtnDelete)
	ON_BN_CLICKED(IDC_RADIO_MESSAGES, OnRadioMessages)
	ON_BN_CLICKED(IDC_RADIO_TIMES, OnRadioTimes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaintenanceDlg message handlers

void CMaintenanceDlg::OnBtnDelete() 
    {

   	POSITION pos = m_ListRecsCtrl.GetFirstSelectedItemPosition();
 	if (pos == NULL)
       	return;

    while (pos)
        {
        pos = m_ListRecsCtrl.GetFirstSelectedItemPosition();
	    int nItem = m_ListRecsCtrl.GetNextSelectedItem(pos);
        CString name = m_ListRecsCtrl.GetItemText(nItem, 1);
        if (DeleteFile(name))
            {
		    m_ListRecsCtrl.DeleteItem(nItem);
            }
        }
    }

void CMaintenanceDlg::OnRadioMessages() 
    {
    PopulateMessagesList();	
    }
    

void CMaintenanceDlg::OnRadioTimes() 
    {
    PopulateTimesList();	
    }

BOOL CMaintenanceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
// create, initialize, and hook up image list
     int cx, cy;

    cx= GetSystemMetrics(SM_CXSMICON);
    cy= GetSystemMetrics(SM_CYSMICON);

    m_ImageList.Create(cx,cy, ILC_MASK, 8, 8);

    m_ImageList.Add(theApp.LoadIcon(IDI_ICON_TIME));
	m_ImageList.Add(theApp.LoadIcon(IDI_ICON_MSG));
    m_ListRecsCtrl.SetImageList(&m_ImageList, LVSIL_SMALL);
    CRect rect;
    m_ListRecsCtrl.GetClientRect(rect);

    m_ListRecsCtrl.InsertColumn(0, "Name", LVCFMT_LEFT, (rect.Width() -20)/2);
    m_ListRecsCtrl.InsertColumn(1, "File", LVCFMT_LEFT, (rect.Width() -20)/2);
 
    // Enable the full row selection and the drag drop of headers.
    DWORD styles = LVS_EX_FULLROWSELECT;
    // Use macro since this is new and not in MFC.
    ListView_SetExtendedListViewStyleEx(m_ListRecsCtrl.GetSafeHwnd(), styles, styles );

    // Reload any stored stuff
	m_RadioMessages = 0;
    if (PopulateMessagesList() == 0 ) 
        {        
        if (PopulateTimesList() > 0)
            m_RadioMessages = 0;
        }
    UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
    }

int CMaintenanceDlg::PopulateMessagesList()
    {
    m_ListRecsCtrl.DeleteAllItems();
    CFileFind finder;
    bool result;
    result = !!finder.FindFile(theApp.CustomSoundDir() + "\\*.wav");

    while (result)
        {
        result = !!finder.FindNextFile();
        CString fname = finder.GetFilePath();
        CString name = finder.GetFileName().Left(finder.GetFileName().GetLength() - 4);
        if (name.Left(6) != "cdhour" && 
            name.Left(5) != "cdnum" &&
            !theApp.m_Recs.RecordExists(name))
            {
            int item =  m_ListRecsCtrl.InsertItem(m_ListRecsCtrl.GetItemCount(), name, 1);
            m_ListRecsCtrl.SetItemText(item, 1, fname);
            }
        }
    return m_ListRecsCtrl.GetItemCount();
    }

int CMaintenanceDlg::PopulateTimesList()
    {
    m_ListRecsCtrl.DeleteAllItems();
    CFileFind finder;
    bool result;
    result = !!finder.FindFile(theApp.CustomSoundDir() + "\\*.wav");

    while (result)
        {
        result = !!finder.FindNextFile();
        CString fname = finder.GetFilePath();
        CString name = finder.GetFileName().Left(finder.GetFileName().GetLength() - 4);
        if (name.Left(6) == "cdhour" || 
            name.Left(5) == "cdnum")
            {
            name.Replace("cdhour", "hour: ");
            name.Replace("cdnum", "number: ");
            int item =  m_ListRecsCtrl.InsertItem(m_ListRecsCtrl.GetItemCount(), name, 1);
            m_ListRecsCtrl.SetItemText(item, 1, fname);
            }
        }
    return m_ListRecsCtrl.GetItemCount();
    }
