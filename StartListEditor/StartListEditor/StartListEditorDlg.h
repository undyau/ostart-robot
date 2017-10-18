
// StartListEditorDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <set>


// CStartListEditorDlg dialog
class CStartListEditorDlg : public CDialogEx
{
// Construction
public:
	CStartListEditorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STARTLISTEDITOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStartlist();
	afx_msg void OnBnClickedButtonOutput();
	afx_msg void OnBnClickedOk();
	CString m_StartList;
	CEdit m_StartListCtrl;
	CComboBox m_StartsCtrl;

private:
	bool LoadStarts();
	bool WriteStarts();
	std::set<CString> m_Starts;
public:
	CEdit m_OutputCtrl;
	afx_msg void OnBnClickedButtonProcess();
	};
