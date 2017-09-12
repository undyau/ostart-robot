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
};

