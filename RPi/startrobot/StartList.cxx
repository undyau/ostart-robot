#include "StartList.hpp"
#include "tinyxml2.h"

CStartList::CStartList(string m_FileName, string a_RootDir):m_RootDir(a_RootDir)
{
    m_FileName = a_RootDir + m_FileName.substr(m_FileName.find_last_of("/\\") + 1);
}

bool CStartList::IsNameFile(string a_FileName)
{
    return m_Files.find(a_FileName) != m_Files.end();
}

timeval CStartList::ISO8601ToTimeval(string a_Text)
{  
    timeval tm;
    int nHour, nMinute, nSecond;
    nHour = atoi(a_Text.substr(11, 2));
	nMinute = atoi(a_Text.substr(14, 2));
	nSecond = atoi(a_Text.substr(17, 2));
	
	tm.tv_usec = 0;
	tm.tv_sec = ((nHour*60) + nMinute)*60 + nSecond;
	return tm;
}

string CStartList::NormaliseName(string & const a_Name)
{
    string result(a_Name);

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

bool CStartList::FileExists(string a_File)
{
  struct stat buffer;   
  return (stat (a_File.c_str(), &buffer) == 0); 
}

bool CStartList::CheckNameSound(string a_Name)
{
    string file = m_RootDir + "/Custom/" + a_Name + ".wav"
    if (!FileExists(file))
       {
       cout << "Missing Sound file for name " << a_Name << endl;
       return false;           
       }
    m_Files.insert(file);
}

bool CStartList::AddStarter(string const & a_GivenName, string const & a_FamilyName, string const & a_StartTime)
	{
	timeval tm;
	if (!ISO8601ToTimeval(a_StartTime, tm))
		return false;
	string name(NormaliseName(a_GivenName + " " + a_FamilyName));
	m_StartTimes.insert(std::pair<CTimeVal, string>(tm, name));

	if (!CheckNameSound(name))
		return false;

	return true;
	}

bool CStartList::Init()
{
    if (m_FileName.isempty())
       return true;
       
    m_Files.empty();
    m_StartTimes.empty();
       
    tinyxml2::XMLDocument doc;
	doc.LoadFile(m_FileName);

	tinyxml2::XMLElement* root = doc.FirstChildElement("StartList");
	if (root == nullptr)
		{
		cout << "Couldn't process file : " + m_FileName + ". It doesn't contain XML element StartList.";
		return false;
		}

    // Create sound for empty time slots
	if (!CheckNameSound("No Starters"))
		return false;

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
				stime = start->FirstChildElement("StartTime")->GetText();

			if (!AddStarter(gname, fname, stime))
				return false;

			personStart = personStart->NextSiblingElement("PersonStart");
			}
		oclass = oclass->NextSiblingElement("ClassStart");
	   } 
    return true;
}
