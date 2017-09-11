#pragma once
#include "afxwin.h"
#include <vector>


// CPropertiesDlg dialog

class CRecordingPropsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRecordingPropsDlg)

public:
	CRecordingPropsDlg(CWnd* pParent = NULL, CString m_VoiceName = CString(""));   // standard constructor
	virtual ~CRecordingPropsDlg();
	CString GetVoice() { return m_VoiceName; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RECORDING_PROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_VoiceCtrl;

private:
	void EnumerateVoices();
	std::vector<CString> m_Voices;
	CString m_VoiceName;
public:
//	char m_Voice;

	virtual BOOL OnInitDialog();
};
