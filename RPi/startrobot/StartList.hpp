#include <string>
#include <map>
#include <set>
#include <sys/time.h>
#include "TimeVal.hpp"
using namespace std;

class CStartList
{
public:
    CStartList(string a_FileName, string a_RootDir);
    ~CStartList(){};
    bool Init();   
    bool IsNameFile(string a_FileName); 

private:
    std::multimap<CTimeVal, string> m_StartTimes;   //start time, normalised name.
    std::set<string> m_Files;
    string m_FileName;
    string m_RootDir;
	bool AddStarter(string const & a_GivenName, string const & a_FamilyName, string const & a_StartTime);
	bool CheckNameSound(string a_Name);
	timeval ISO8601ToTimeval(string a_Text);
    string NormaliseName(string a_Name); 
};
