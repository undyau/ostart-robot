
// StartListEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StartListEditor.h"
#include "StartListEditorDlg.h"
#include "afxdialogex.h"
#include "tinyxml2.h"
#include <set>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStartListEditorDlg dialog



CStartListEditorDlg::CStartListEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STARTLISTEDITOR_DIALOG, pParent)
	, m_StartList(_T(""))
	{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStartListEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_STARTLIST, m_StartListCtrl);
	DDX_Control(pDX, IDC_COMBO_STARTS, m_StartsCtrl);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_OutputCtrl);
	}

BEGIN_MESSAGE_MAP(CStartListEditorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_STARTLIST, &CStartListEditorDlg::OnBnClickedButtonStartlist)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, &CStartListEditorDlg::OnBnClickedButtonOutput)
	ON_BN_CLICKED(IDOK, &CStartListEditorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS, &CStartListEditorDlg::OnBnClickedButtonProcess)
END_MESSAGE_MAP()


// CStartListEditorDlg message handlers

BOOL CStartListEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CStartListEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CStartListEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CStartListEditorDlg::OnBnClickedButtonStartlist()
	{
	char szFilters[] = "XML Files (*.xml)|*.xml|All Files (*.*)|*.*||";
	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg(TRUE, "xml", "*.xml",
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, this);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal()
	// returns IDOK.
	if (fileDlg.DoModal() == IDOK)
		{
		CString m_strPathname = fileDlg.GetPathName();
		m_StartListCtrl.SetWindowText(m_strPathname);
		LoadStarts();
		}
	}


bool CStartListEditorDlg::LoadStarts()
	{
	m_StartsCtrl.ResetContent();
	m_Starts.empty();

	tinyxml2::XMLDocument doc;
	CString fname;
	m_StartListCtrl.GetWindowText(fname);

	doc.LoadFile((LPCTSTR)fname);

	tinyxml2::XMLElement* root = doc.FirstChildElement("StartList");
	if (root == nullptr)
		{
		AfxMessageBox(CString("Couldn't process file : ") + fname + ". It doesn't contain XML element StartList.", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}

	// Iterate over all classes
	tinyxml2::XMLElement* oclass = root->FirstChildElement("ClassStart");
	if (oclass == nullptr)
		{
		AfxMessageBox(CString("Couldn't process file : ") + fname + ". It doesn't contain XML element ClassStart.", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}
	while (oclass)
		{
		tinyxml2::XMLElement* startName = oclass->FirstChildElement("StartName");
		if (startName)
			{
			CString start = CString(startName->GetText());
			m_Starts.insert(start);
			}
		oclass = oclass->NextSiblingElement("ClassStart");
		}

	for (auto i = m_Starts.begin(); i != m_Starts.end(); i++)
		m_StartsCtrl.AddString(*i);
	return true;
	}

bool CStartListEditorDlg::WriteStarts()
	{
	//std::set<tinyxml2::XMLElement*> 
	CString start;
	m_StartsCtrl.GetWindowTextA(start);
	if (start.IsEmpty())
		{
		AfxMessageBox("No start chosen.", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}


	tinyxml2::XMLDocument doc;
	CString fname;
	m_StartListCtrl.GetWindowText(fname);
	doc.LoadFile((LPCTSTR)fname);

	tinyxml2::XMLElement* root = doc.FirstChildElement("StartList");
	if (root == nullptr)
		{
		AfxMessageBox(CString("Couldn't process file : ") + fname + ". It doesn't contain XML element StartList.", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}

	// Iterate over all classes
	tinyxml2::XMLElement* oclass = root->FirstChildElement("ClassStart");
	if (oclass == nullptr)
		{
		AfxMessageBox(CString("Couldn't process file : ") + fname + ". It doesn't contain XML element ClassStart.", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}
	while (oclass)
		{
		tinyxml2::XMLElement* startName = oclass->FirstChildElement("StartName");
		tinyxml2::XMLElement* next = oclass->NextSiblingElement("ClassStart");
		if (startName && startName->GetText() != start)
			{
			oclass->DeleteChildren();
			}
		oclass = next;
		}
	CString ofile;
	m_OutputCtrl.GetWindowText(ofile);
	if (doc.SaveFile(ofile) != tinyxml2::XML_SUCCESS)
		{
		AfxMessageBox(CString("Couldn't save to file : ") + ofile + ". It doesn't contain XML element ClassStart.", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}
	}


void CStartListEditorDlg::OnBnClickedButtonOutput()
	{
	char szFilters[] = "XML Files (*.xml)|*.xml|All Files (*.*)|*.*||";
	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg(FALSE, "xml", "*.xml",
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, this);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal()
	// returns IDOK.
	if (fileDlg.DoModal() == IDOK)
		{
		CString m_strPathname = fileDlg.GetPathName();
		
		CString fname;
		m_StartListCtrl.GetWindowText(fname);
		if (fname == m_strPathname)
			{
			AfxMessageBox("Can't  overwrite source file " + fname,  MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
			return;
			}
		m_OutputCtrl.SetWindowText(m_strPathname);
		}
	}


void CStartListEditorDlg::OnBnClickedOk()
	{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
	}


void CStartListEditorDlg::OnBnClickedButtonProcess()
	{
	if (WriteStarts())
		AfxMessageBox("It is done", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
	}
