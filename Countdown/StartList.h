#pragma once
#include <map>
class CStartList
{
public:
	CStartList();
	~CStartList();
	bool Load(CString a_FileName);

private:
	std::map<CString, CString> m_StartTimes;   //start time, comma-separated rectified name list
	bool AddStarter(CString const & a_GivenName, CString const & a_FamilyName, CString const& a_StartTime);
	bool GotNameSound(CString const& a_Name);
	bool CreateNameSound(CString const& a_Name);
	CString NormaliseName(CString const & a_Name);
	CString ToFileName(CString const & a_Name);
};

