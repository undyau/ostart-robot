// CountdownDlg.h : header file
//

#if !defined(AFX_COUNTDOWNDLG_H__8D766B96_47CD_4D7A_9D99_2435D8410DC3__INCLUDED_)
#define AFX_COUNTDOWNDLG_H__8D766B96_47CD_4D7A_9D99_2435D8410DC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DynamicLED.h"
#include "ListCtrlEx.h"
#include "FileOperations.h"
/////////////////////////////////////////////////////////////////////////////
// CCountdownDlg dialog

class CCountdownDlg : public CDialog
{
// Construction
public:
	CCountdownDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCountdownDlg)
	enum { IDD = IDD_COUNTDOWN_DIALOG };
	CButton	m_CancelBtnCtrl;
	CEdit	m_TimingTTCtrl;
	CEdit	m_TimingSSCtrl;
	CEdit	m_NextPlusMMCtrl;
	CButton	m_AmplifyCtrl;
	CButton	m_DampenCtrl;
	CButton	m_ModifyCtrl;
	CButton	m_AddModeCtrl;
	CButton	m_AddAfterCtrl;
	CButton	m_PlayRecCtrl;
	CEdit	m_TimingMMCtrl;
	CComboBox	m_TimingPrePostCtrl;
	CComboBox	m_RelativeCtrl;
	CEdit	m_LastMMCtrl;
	CEdit	m_LastHHCtrl;
	CEdit	m_FirstMMCtrl;
	CEdit	m_FirstHHCtrl;
	CButton	m_AddBtnCtrl;
	CButton	m_PlayCtrl;
	CButton	m_DeleteCtrl;
	CButton	m_CreateCtrl;
	CButton	m_BrowseCtrl;
	CEdit	m_FileNameCtrl;
	CDynamicLED	m_LedCtrl;
	CStatic	m_MsgNameCtrl;
	CButton	m_FreqRadioCtrl;
	CButton	m_ContentRadioCtrl;
	CButton	m_RecordingCtrl;
	CListCtrlEx	m_ListMsgs;
	CComboBox	m_NewMsgCtrl;
	CComboBox	m_ContentMsgCtrl;
	CEdit	m_FreqSSCtrl;
	CEdit	m_FreqMMCtrl;
	CComboBox	m_FreqMsgCtrl;
	CString	m_ContentFormat;
	CString	m_ContentMsg;
	CString	m_FreqMsg;
	CString	m_FreqMM;
	CString	m_NewMsg;
	CString	m_LastHH;
	CString	m_LastMM;
	CString	m_FirstHH;
	CString	m_FirstMM;
	int		m_ContentRadio;
	int		m_FreqRadio;
	int		m_TimingRadio;
	CString	m_FileName;
	CString	m_TimingMM;
	CString	m_TimingSS;
	CString	m_TimingTT;
	CString	m_NextPlusMM;
	CString	m_FreqSS;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCountdownDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCountdownDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonCreate();
	afx_msg void OnButtonRecord();
	afx_msg void OnRadioContentMsg();
	afx_msg void OnRadioContentTime();
	afx_msg void OnRadioFreqAfterMsg();
	afx_msg void OnRadioFreqEvery();
	afx_msg void OnRadioTimingFinish();
	afx_msg void OnRadioTimingStart();
	afx_msg void OnButtonAdd();
	afx_msg void OnSelchangeComboFreqMsg();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonTest();
	afx_msg void OnButtonBrowse();
	afx_msg void OnChangeEditFilename();
	afx_msg void OnEditchangeComboRecordMsg();
	afx_msg void OnClickListMessages(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteitemListMessages(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusListMessages(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAmplify();
	afx_msg void OnSelchangeComboRecordMsg();
	afx_msg void OnButtonPlay();
	afx_msg void OnSelchangeComboRelative();
	afx_msg void OnChangeEditFreqHh();
	afx_msg void OnChangeEditFreqMm();
	afx_msg void OnChangeEditFreqSs();
	afx_msg void OnMenuAbout();
	afx_msg void OnMenuClock();
	afx_msg void OnMenuCreate();
	afx_msg void OnMenuExit();
	afx_msg void OnMenuHelpContents();
	afx_msg void OnMenuMaintenance();
	afx_msg void OnClose();
    afx_msg void OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu);
	afx_msg void OnButtonPlayrec();
	afx_msg void OnButtonAddafter();
	afx_msg void OnButtonDampen();
	afx_msg void OnButtonModify();
	afx_msg void OnMenuExport();
	afx_msg void OnMenuImport();
	afx_msg void OnCancelBtn();
	//}}AFX_MSG
	afx_msg LRESULT OnClosedSoundFile(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	CRecordSound* m_RecordThread;

private:
    boolean m_Recording;
    bool m_InAdd;
    bool m_InCreate;
    bool m_InExit;
	bool m_ModifyMode;
    CImageList m_ImageList;
    bool m_LargeOffsets;
	bool m_InterimChange;
    
    void PopulateCombos();
    void EnableCtrls();
    bool IsChecked(int a_ID);
    CString TimeString(const CString& a_HH, const CString& a_MM, const CString& a_SS, CString a_TT = "0") const;
    void AddListEntry(CRecording* a_Rec);
    void ShrinkWindow(int a_DlgID);
	void RedrawList();
	void ModifyVolume(int a_PctChange) ;
	void ModifyRec();
	void InitialiseData();
	bool CopyAllUsedSounds(CFileOperation& fo, CString folder);
    CMenu m_Menu;
 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COUNTDOWNDLG_H__8D766B96_47CD_4D7A_9D99_2435D8410DC3__INCLUDED_)
