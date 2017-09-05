#include <string>
#include <map>
using namespace std;

class CSoundLibrary
{
public:
    CSoundLibrary(string a_Dir);
    ~CSoundLibrary();
    string GetAnnouncementFile(string a_Title);
    float GetAnnouncementDuration(string a_Title);
    string GetTimeSndFile(string a_Title);
    float GetTimeSndDuration(string a_Title);  
    void ProcessFile(string a_File);  

private:
	std::map<string, float> m_TimeSndDurations;
	std::map<string, float> m_AnnouncementDurations;
	std::map<string, string> m_TimeSndFiles;
	std::map<string, string> m_AnnouncementFiles;
	
	bool IsTimeFile(string a_File);	
	float GetDuration(string a_File);
	
};
