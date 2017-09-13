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
	m_StartTimes.empty();
	tinyxml2::XMLDocument doc;
	doc.LoadFile(a_FileName);

	tinyxml2::XMLElement* root = doc.FirstChildElement("StartList");
	if (root == nullptr)
		{
		AfxMessageBox("Couldn't process file : " + a_FileName + ". It doesn't contain XML element StartList.", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}

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

			AddStarter(gname, fname, stime);

			personStart = personStart->NextSiblingElement("PersonStart");
			}
		oclass = oclass->NextSiblingElement("ClassStart");
		}

	return true;
}

bool CStartList::AddStarter(CString const & a_GivenName, CString const & a_FamilyName, CString const & a_StartTime)
	{
	if (m_StartTimes.find(a_StartTime) != m_StartTimes.end())
		m_StartTimes[a_StartTime] = m_StartTimes[a_StartTime] + "," + a_GivenName + " " + a_FamilyName;
	else
		m_StartTimes[a_StartTime] = a_GivenName + " " + a_FamilyName;

	if (!GotNameSound(a_GivenName))
		if (!CreateNameSound(a_GivenName))
			return false;
	if (!GotNameSound(a_FamilyName))
		if (!CreateNameSound(a_FamilyName))
			return false;

	return true;
	}

bool CStartList::GotNameSound(CString const & a_Name)
	{
	CString name = ToFileName(NormaliseName(a_Name));
	return FileExists(theApp.CustomNamesDir() + "\\" + name + ".wav");
	}

bool CStartList::CreateNameSound(CString const & a_Name)
	{
	theApp.CreateNameSound(a_Name, ToFileName(NormaliseName(a_Name)));
	return false;
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


/*
<PersonStart>
<Person sex="M">
<Id>5075</Id>
<Name>
<Family>Harding</Family>
<Given>Stephen</Given>
</Name>
<BirthDate>1999-01-01</BirthDate>
</Person>
<Organisation>
<Id>247</Id>
<Name>NZ L</Name>
<ShortName>NZ L</ShortName>
<Country code="NZL">NZL</Country>
</Organisation>
<Start raceNumber="1">
<BibNumber>128</BibNumber>
<StartTime>2017-09-24T10:26:00.000</StartTime>
<ControlCard>2033334</ControlCard>
<AssignedFee>
<Fee>
<Name>Actual</Name>
<Amount>27.00</Amount>
</Fee>
</AssignedFee>
</Start>
</PersonStart>
*/