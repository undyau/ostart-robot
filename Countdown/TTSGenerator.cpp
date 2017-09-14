#include "stdafx.h"
#include "TTSGenerator.h"
#include <sapi.h>
#pragma warning(disable:4996)
#include <sphelper.h>
#pragma warning(default:4996)
#include "CountDown.h"

extern CCountdownApp theApp;


CTTSGenerator::CTTSGenerator()
	{
	}


CTTSGenerator::~CTTSGenerator()
	{
	}

bool CTTSGenerator::GenerateSound(CString const & a_Text, CString const & a_FileName)
	{

	HRESULT				hr = S_OK;
	CComPtr <ISpVoice>		cpVoice;
	CComPtr <ISpStream>		cpStream;
	CSpStreamFormat			cAudioFmt;
	CComPtr <ISpObjectToken>	cpToken;
	CComPtr <IEnumSpObjectTokens>	cpEnum;
	ULONG   ulCount = 0;

	//Create a SAPI Voice
	hr = cpVoice.CoCreateInstance(CLSID_SpVoice);
	if (!SUCCEEDED(hr))
		{
		AfxMessageBox("cpVoice.CoCreateInstance failed", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}

	//Set the audio format
	hr = cAudioFmt.AssignFormat(SPSF_22kHz16BitMono);
	if (!SUCCEEDED(hr))
		{
		AfxMessageBox("cAudioFmt.AssignFormat failed", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}

	//Enumerate voice tokens with attribute "Name=Microsoft Sam"
	CStringW voiceW = theApp.m_Recs.GetVoice();
	/*if (voiceW.Find(L" - ") > 0)
		voiceW = voiceW.Left(voiceW.Find(L" - "));*/
	CString voice = voiceW;
	hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum);
	if (!SUCCEEDED(hr))
		{
		AfxMessageBox("SpEnumTokens failed searching for " + voice, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}

	hr = cpEnum->GetCount(&ulCount);
	if (!SUCCEEDED(hr))
		{
		AfxMessageBox("cpEnum->GetCount failed ", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}

	// Obtain a list of available voice tokens, sfind the one that we want
	while (SUCCEEDED(hr) && ulCount--)
		{
		cpToken.Release();
		if (SUCCEEDED(hr))
			hr = cpEnum->Next(1, &cpToken, NULL);

		if (SUCCEEDED(hr)) {
			WCHAR	*desc;
			SpGetDescription(cpToken, &desc, SpGetUserDefaultUILanguage());
			if (CString(desc) == voice)
				{
				break;
				}
			}
		}
	if (!SUCCEEDED(hr))
		{
		AfxMessageBox("cpEnum->Next failed to find voice " + voice, MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		cpEnum.Release();
		cpToken.Release();
		return false;
		}

	cpEnum.Release();

	//set the voice
	hr = cpVoice->SetVoice(cpToken);
	if (!SUCCEEDED(hr))
		{
		AfxMessageBox("cpVoice->SetVoice failed", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}

	//Call SPBindToFile, a SAPI helper method,  to bind the audio stream to the file
	hr = SPBindToFile(a_FileName, SPFM_CREATE_ALWAYS, &cpStream, &cAudioFmt.FormatId(), cAudioFmt.WaveFormatExPtr());
	if (!SUCCEEDED(hr))
		{
		AfxMessageBox("SPBindToFile failed", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}

	//set the output to cpStream so that the output audio data will be stored in cpStream
	hr = cpVoice->SetOutput(cpStream, TRUE);
	if (!SUCCEEDED(hr))
		{
		AfxMessageBox("cpVoice->SetOutput failed", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}

	//Speak the text "hello world" synchronously
	hr = cpVoice->Speak(CT2CW(a_Text), SPF_DEFAULT, NULL);
	if (!SUCCEEDED(hr))
		{
		AfxMessageBox("cpVoice->Speak " + a_Text + " failed", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}

	//close the stream
	hr = cpStream->Close();
	if (!SUCCEEDED(hr))
		{
		AfxMessageBox("cpStream->Close() failed", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}

	//Release the stream and voice object
	cpStream.Release();
	cpVoice.Release();

	return true;
	}
