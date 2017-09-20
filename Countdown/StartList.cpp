#include "stdafx.h"
#include "StartList.h"
#include "tinyxml2.h"
#include "utils.h"
#include "Countdown.h"

extern CCountdownApp theApp;

CStartList::CStartList()
{
}


CStartList::~CStartList()
{
}

bool CStartList::Load(CString a_FileName)
{
	CWaitCursor wait;
	m_StartTimes.empty();
	tinyxml2::XMLDocument doc;
	doc.LoadFile(a_FileName);

	tinyxml2::XMLElement* root = doc.FirstChildElement("StartList");
	if (root == nullptr)
		{
		AfxMessageBox("Couldn't process file : " + a_FileName + ". It doesn't contain XML element StartList.", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}

    // Create sound for empty time slots
	if (!GotNameSound("No Starters"))
		if (!CreateNameSound("No Starters"))
			return false;

	// Create custom time sounds using this voice
	if (!CreateTimeSounds())
		return false;

	// Iterate over all classes
	tinyxml2::XMLElement* oclass = root->FirstChildElement("ClassStart");
	if (oclass == nullptr)
		{
		AfxMessageBox("Couldn't process file : " + a_FileName + ". It doesn't contain XML element ClassStart.", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}
	while (oclass)
		{
		tinyxml2::XMLElement* personStart = oclass->FirstChildElement("PersonStart");
		while (personStart)
			{
			CString fname, gname, stime;
			if (personStart->FirstChildElement("Person"))
				{
				if (personStart->FirstChildElement("Person")->FirstChildElement("Name"))
					{
					
					tinyxml2::XMLElement* pname = personStart->FirstChildElement("Person")->FirstChildElement("Name");
					if (pname->FirstChildElement("Family"))
						fname = CString(pname->FirstChildElement("Family")->GetText());
					if (pname->FirstChildElement("Given"))
						gname = CString((pname->FirstChildElement("Given")->GetText()));
					}
				}
			tinyxml2::XMLElement* start = personStart->FirstChildElement("Start");
			if (start && start->FirstChildElement("StartTime"))
				stime = start->FirstChildElement("StartTime")->GetText();

			if (!AddStarter(gname, fname, stime))
				return false;

			personStart = personStart->NextSiblingElement("PersonStart");
			}
		oclass = oclass->NextSiblingElement("ClassStart");
		}

	return true;
}

std::vector<CHighTime> CStartList::StartTimes()
	{
	std::vector<CHighTime> result;
	for (auto it = m_StartTimes.begin(), end = m_StartTimes.end(); it != end; it = m_StartTimes.upper_bound(it->first))
		result.push_back (it->first);
	return result;
	}

std::vector<CString> CStartList::StartersAtTime(CHighTime a_Time)
	{
	std::vector<CString> result;
	std::pair <std::multimap<CHighTime, CString>::iterator, std::multimap<CHighTime, CString>::iterator> ret;
	ret = m_StartTimes.equal_range(a_Time);
	for (std::multimap<CHighTime, CString>::iterator it = ret.first; it != ret.second; ++it)
		result.push_back(it->second);
	return result;
	}

CString CStartList::FileNameOfName(CString a_Name)
	{
	return theApp.CustomNamesDir() + "\\" + ToFileName(NormaliseName(a_Name)) + ".wav";
	}

bool CStartList::AddStarter(CString const & a_GivenName, CString const & a_FamilyName, CString const & a_StartTime)
	{
	CHighTime tm;
	if (!ToCHighTime(a_StartTime, tm))
		return false;
	CString name(a_GivenName + " " + a_FamilyName);
	m_StartTimes.insert(std::pair<CHighTime, CString>(tm, name));
	TRACE2("Added starter %s for %s\n",name, HighTimeAsStr(tm));

	if (!GotNameSound(name))
		if (!CreateNameSound(name))
			return false;

	return true;
	}

bool CStartList::GotNameSound(CString const & a_Name)
	{
	return FileExists(FileNameOfName(a_Name));
	}

bool CStartList::CreateNameSound(CString const & a_Name)
	{
	return theApp.CreateSoundFile(a_Name, FileNameOfName(a_Name));
	}

CString CStartList::NormaliseName(CString const & a_Name)
	{
	CString name(a_Name);
	name.Replace("&apos;", "'");
	name.Replace("&amp;", "&");
	name.Replace("&quot;", "\"");
	name.Replace("&lt;", "<");
	name.Replace("&gt;", ">");

	return name;
	}

CString CStartList::ToFileName(CString const & a_Name)
	{
	CString result(a_Name);

	result.Replace('<','_');
	result.Replace('>', '_');
	result.Replace(':', '_');
	result.Replace('/', '_');
	result.Replace('\\', '_');
	result.Replace('|', '_');
	result.Replace('?', '_');
	result.Replace('*', '_');
	return result;
	}


// Create custom time sounds using this voice
bool CStartList::CreateTimeSounds()
	{
	std::map<CString, CString> targets;
	CString path = theApp.CustomSoundDir() + "\\";
	CString suffix = ".wav";

	targets["cdhour00"] = "midnight";
	targets["cdhour12"] = "noon";
	targets["cdnum00"] = "zero";
	CString t1,t2;
	for (auto i = 0; i < 10; i++)
		{		
		t1.Format("o%d", i);
		t2.Format("cdnum0%d", i);
		targets[t2] = t1;
		}
	for (auto i = 1; i < 60; i++)
		{
		t1.Format("%d", i);
		t2.Format("cdnum%d", i);
		targets[t2] = t1;
		}
	for (auto i = 1; i < 12; i++)
		{
		t1.Format("%d o'clock", i);
		t2.Format("cdhour%02d", i);
		targets[t2] = t1;
		t2.Format("cdhour%02d", i+12);
		targets[t2] = t1;
		}

	for (auto i = targets.begin(); i != targets.end(); i++)
		if (!FileExists(path + i->first + suffix))
			if (!theApp.CreateSoundFile(i->second, path + i->first + suffix))
				return false;
	return true;
	}