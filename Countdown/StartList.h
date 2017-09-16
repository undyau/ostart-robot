#pragma once
#include <map>
#include <vector>
#include "hightime.h"

class CStartList
{
public:
	CStartList();
	~CStartList();
	bool Load(CString a_FileName);
	std::vector<CHighTime> StartTimes();
	std::vector<CString> StartersAtTime(CHighTime a_Time);
	CString FileNameOfName(CString a_Name);
	

private:
	std::multimap<CHighTime, CString> m_StartTimes;   //start time, Given name + family name.
	bool AddStarter(CString const & a_GivenName, CString const & a_FamilyName, CString const& a_StartTime);
	bool GotNameSound(CString const& a_Name);
	bool CreateNameSound(CString const& a_Name);
	CString NormaliseName(CString const & a_Name);
	CString ToFileName(CString const & a_Name);
};

