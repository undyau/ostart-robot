#if !defined(AFX_RECORDTIMESDLG_H__821FA0CF_B46A_4A26_8E38_89D858558C93__INCLUDED_)
#define AFX_RECORDTIMESDLG_H__821FA0CF_B46A_4A26_8E38_89D858558C93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordTimesDlg.h : header file
//

#include "DynamicLED.h"
#include "ColorStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CRecordTimesDlg dialog

class CRecordTimesDlg : public CDialog
{
// Construction
public:
	CRecordTimesDlg(CWnd* pParent, CRecordSound* a_RecordThread);

// Dialog Data
	//{{AFX_DATA(CRecordTimesDlg)
	enum { IDD = IDD_DIALOG_RECORDTIMES };
	CButton	m_SequentialCtrl;
	CButton	m_RecordingCtrl;
	CSpinButtonCtrl	m_SpinNumberCtrl;
	CDynamicLED	m_LedCtrl;
	CColorStatic	m_DisplayNumberCtrl;
	CString	m_DisplayNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordTimesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecordTimesDlg)
	afx_msg void OnButtonPlay();
	afx_msg void OnButtonRecord();
	afx_msg void OnDeltaposSpinNumber(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnClosedSoundFile(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
    int m_Number;
    bool m_Recording;
    CRecordSound* m_RecordThread;

    CString NumberToText(int a_Number);
    CString GetRecordingName(int a_Number);
    void IncNumber(int a_Increment);
    
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDTIMESDLG_H__821FA0CF_B46A_4A26_8E38_89D858558C93__INCLUDED_)
