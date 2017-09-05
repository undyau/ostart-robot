#if !defined(AFX_MAINTENANCEDLG_H__D2101D76_3951_4254_99FF_7B9C275FDD44__INCLUDED_)
#define AFX_MAINTENANCEDLG_H__D2101D76_3951_4254_99FF_7B9C275FDD44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaintenanceDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMaintenanceDlg dialog

class CMaintenanceDlg : public CDialog
{
// Construction
public:
	CMaintenanceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMaintenanceDlg)
	enum { IDD = IDD_DIALOG_MAINTAIN };
	CButton	m_RadioCtrl;
	CButton	m_DeleteCtrl;
	CListCtrl	m_ListRecsCtrl;
	int		m_RadioMessages;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaintenanceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMaintenanceDlg)
	afx_msg void OnBtnDelete();
	afx_msg void OnRadioMessages();
	afx_msg void OnRadioTimes();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    CImageList m_ImageList;

    int PopulateMessagesList();
    int PopulateTimesList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINTENANCEDLG_H__D2101D76_3951_4254_99FF_7B9C275FDD44__INCLUDED_)
