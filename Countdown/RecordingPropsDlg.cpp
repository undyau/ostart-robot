// RecordingPropsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Countdown.h"
#include "RecordingPropsDlg.h"
#include "afxdialogex.h"
#include <sapi.h>
#pragma warning(disable:4996)
#include <sphelper.h>
#pragma warning(default:4996)


// CRecordingPropsDlg dialog

IMPLEMENT_DYNAMIC(CRecordingPropsDlg, CDialogEx)

CRecordingPropsDlg::CRecordingPropsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PROPERTIES, pParent)
	, m_Voice(0)
{

}

CRecordingPropsDlg::~CRecordingPropsDlg()
{
}

void CRecordingPropsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_VOICE, m_VoiceCtrl);
}

void CRecordingPropsDlg::EnumerateVoices()
{
	HRESULT hr = S_OK;

	CComPtr<ISpObjectTokenCategory> cpSpCategory = NULL;
	CComPtr<ISpObjectToken>        cpToken;
	CComPtr<IEnumSpObjectTokens>   cpEnum;
	CComPtr<ISpVoice>              cpVoice;
	ULONG                          ulCount = 0;

	m_Voices.empty();

	if (SUCCEEDED(hr))
		hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum);

	if (SUCCEEDED(hr))
		hr = cpEnum->GetCount(&ulCount);

	// Obtain a list of available voice tokens, set the voice to the token, and call Speak
	while (SUCCEEDED(hr) && ulCount--)
	{
		cpToken.Release();
		if (SUCCEEDED(hr))
			hr = cpEnum->Next(1, &cpToken, NULL);

		if (SUCCEEDED(hr)) {
			WCHAR	*desc;
			SpGetDescription(cpToken, &desc, SpGetUserDefaultUILanguage());
#ifdef	UNICODE
			m_Voices.push_back(CString(desc));
#else
			m_Voices.push_back(CString(desc));
#endif
		}
	}
	cpEnum.Release();
	cpToken.Release();
	/*
	// Create the SAPI voice.
	hr = cpVoice.CoCreateInstance(CLSID_SpVoice);

	if (SUCCEEDED(hr))
	{
	hr = CoCreateInstance(CLSID_SpObjectTokenCategory, NULL, CLSCTX_ALL, __uuidof(ISpObjectTokenCategory), (void**)&cpSpCategory);
	}

	if (SUCCEEDED(hr))
	{
	hr = cpSpCategory->SetId(SPCAT_VOICES, FALSE);
	}

	if (SUCCEEDED(hr))
	{
	// Enumerate the available voices.
	hr = cpSpCategory->EnumTokens(SPCAT_VOICES, NULL, &cpEnum);
	}

	if (SUCCEEDED(hr))
	{
	// Get the number of voices.
	hr = cpEnum->GetCount(&ulCount);
	}

	// Obtain a list of available voice tokens, set
	// the voice to the token, and call Speak.
	while (SUCCEEDED(hr) && ulCount--)
	{
	cpVoiceToken.Release();

	if (SUCCEEDED(hr))
	{
	hr = cpEnum->Next(1, &cpVoiceToken , NULL);
	}

	if (SUCCEEDED(hr))
	{
	CSpDynamicString dstrDesc;
	SpGetDescription(cpVoiceToken, &dstrDesc);
	m_Voices.push_back(CString(dstrDesc));
	}

	if (SUCCEEDED(hr))
	{
	hr = cpVoice->Speak(L"How are you?", SPF_DEFAULT, NULL);
	}

	}

	if (SUCCEEDED(hr))
	{
	// Do more stuff here.
	}*/
}


BEGIN_MESSAGE_MAP(CRecordingPropsDlg, CDialogEx)
END_MESSAGE_MAP()


// CRecordingPropsDlg message handlers


BOOL CRecordingPropsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	EnumerateVoices();
	for (unsigned int i = 0; i < m_Voices.size(); i++)
		m_VoiceCtrl.InsertString(-1, m_Voices.at(i));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
