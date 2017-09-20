#include <string>
#include <map>
using namespace std;

class CSoundLibrary
{
public:
    CSoundLibrary(string a_Dir);
    ~CSoundLibrary();
    void Init();
    string GetAnnouncementFile(string a_Title);
    float GetAnnouncementDuration(string a_Title);
    string GetTimeSndFile(string a_Title);
    float GetTimeSndDuration(string a_Title);  
    string GetNameSndFile(string a_Title);
    float GetNameSndDuration(string a_Title);      
    void ProcessFile(string a_File);  

private:
	std::map<string, float> m_TimeSndDurations;
	std::map<string, string> m_TimeSndFiles;
	std::map<string, string> m_AnnouncementFiles;
	std::map<string, float> m_AnnouncementDurations; 
	std::map<string, float> m_NameSndDurations;  
	std::map<string, string> m_NameSndFiles;  
	
	bool IsTimeFile(string a_File);
  bool IsNameFile(string a_File);	
	float GetDuration(string a_File);
  string m_Dir;
	
};
