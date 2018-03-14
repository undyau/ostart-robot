#include "StartList.hpp"
#include "tinyxml2.h"
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <iostream>

CStartList::CStartList(string a_FileName, string a_RootDir):m_RootDir(a_RootDir)
{
    m_FileName = a_RootDir + "/" + a_FileName.substr(m_FileName.find_last_of("/\\") + 1);
}

bool CStartList::IsNameFile(string a_FileName)
{
    return m_Files.find(a_FileName) != m_Files.end();
}

timeval CStartList::ISO8601ToTimeval(string a_Text)
{  
	// tm is interpreted as being in GMT, but it probably isn't
	// Get the difference from GMT and subtract it
  time_t t = time(NULL);
  struct tm lt = {0};

  localtime_r(&t, &lt);

  timeval tm;
  int nHour, nMinute, nSecond;
  nHour = atoi(a_Text.substr(11, 2).c_str());
	nMinute = atoi(a_Text.substr(14, 2).c_str());
	nSecond = atoi(a_Text.substr(17, 2).c_str());
	tm.tv_usec = 0;
	tm.tv_sec = ((nHour*60) + nMinute)*60 + nSecond - lt.tm_gmtoff ;
	tm.tv_sec += lt.tm_isdst ? 3600 : 0;

	return tm;
}

string CStartList::NormaliseName(string a_Name)
{
  string result(a_Name);

  std::replace( result.begin(), result.end(), '<','_'); 
	std::replace( result.begin(), result.end(),'<','_');
	std::replace( result.begin(), result.end(),'>', '_');
	std::replace( result.begin(), result.end(),':', '_');
	std::replace( result.begin(), result.end(),'/', '_');
	std::replace( result.begin(), result.end(),'\\', '_');
	std::replace( result.begin(), result.end(),'|', '_');
	std::replace( result.begin(), result.end(),'?', '_');
	std::replace( result.begin(), result.end(),'*', '_');  
	std::replace( result.begin(), result.end(),' ', '_'); 
	std::replace( result.begin(), result.end(),'\'', '_');    
	return result;
}

bool CStartList::FileExists(string a_File)
{
  return access( a_File.c_str(), F_OK ) != -1; 
}

bool CStartList::CheckNameSound(string a_Name)
{
    string file = m_RootDir + "/Custom/" + a_Name + ".wav";
    if (!FileExists(file))
       {
       cout << "Missing Sound file for name " << a_Name << " " << file << " not found" << endl;
       return false;           
       }
    m_Files.insert(file);
    return true;
}

bool CStartList::AddStarter(string const & a_GivenName, string const & a_FamilyName, string const & a_StartTime)
	{
	CTimeVal tm = CTimeVal(ISO8601ToTimeval(a_StartTime));
	string name(NormaliseName(a_GivenName + " " + a_FamilyName));
	m_StartTimes.insert(std::pair<string, string>(tm.TimeString(), name));
//cout << "Added starter " << name << " at " << tm.TimeString() << endl;
	if (!CheckNameSound(name))
		return false;

	return true;
	}

bool CStartList::Init()
{
  if (m_FileName.size() == 0)
		{
		cout << "No StartList file supplied." << endl;
		return true;
		}
     
    m_Files.empty();
    m_StartTimes.empty();
       
    tinyxml2::XMLDocument doc;
	doc.LoadFile(m_FileName.c_str());

	tinyxml2::XMLElement* root = doc.FirstChildElement("StartList");
	if (root == nullptr)
		{
		cout << "Couldn't process file : " + m_FileName + ". It doesn't contain XML element StartList.";
		return false;
		}

    // Create sound for empty time slots
	string name("No Starters");
	name = NormaliseName(name);
	CheckNameSound(name);

	// Iterate over all classes
	tinyxml2::XMLElement* oclass = root->FirstChildElement("ClassStart");
	if (oclass == nullptr)
		{
		cout << "Couldn't process file : " + m_FileName + ". It doesn't contain XML element ClassStart.";
		return false;
		}
	while (oclass)
		{
		tinyxml2::XMLElement* personStart = oclass->FirstChildElement("PersonStart");
		while (personStart)
			{
			string fname, gname, stime;
			if (personStart->FirstChildElement("Person"))
				{
				if (personStart->FirstChildElement("Person")->FirstChildElement("Name"))
					{
					
					tinyxml2::XMLElement* pname = personStart->FirstChildElement("Person")->FirstChildElement("Name");
					if (pname->FirstChildElement("Family"))
						fname = string(pname->FirstChildElement("Family")->GetText());
					if (pname->FirstChildElement("Given"))
						gname = string((pname->FirstChildElement("Given")->GetText()));
					}
				}
			tinyxml2::XMLElement* start = personStart->FirstChildElement("Start");
			if (start && start->FirstChildElement("StartTime"))
				{
				stime = start->FirstChildElement("StartTime")->GetText();
				if (!AddStarter(gname, fname, stime))
					return false;
				}
			personStart = personStart->NextSiblingElement("PersonStart");
			}
		oclass = oclass->NextSiblingElement("ClassStart");
	   } 
    cout << "Loaded " << m_StartTimes.size() << " starters" << endl;
    return true;
}
