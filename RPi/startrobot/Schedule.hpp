#include <string>
#include <map>
#include "Chain.hpp"
using namespace std;

class CSchedule
{
public:
	CSchedule(string a_Dir);
	bool Load();
	void PlayNextSound();   // Schedule sounds and run them
	string StartListFile() {return m_StartListFile;}
    }
	
private:
	string m_Dir;
	string m_XmlStr;
	string m_StartListFile;
	int m_FrequencySecs;
	int m_Version;
	
	map<float, shared_ptr<CChain>> m_Program; // each chain with offset
	
	string GetElementVal(size_t a_Pos, string a_Name);
	bool GetElementValBool(size_t a_Pos, string a_Name);	
	int GetElementValInt(size_t a_Pos, string a_Name);
	float GetElementValTimeVal(size_t a_Pos, string a_Name);
	unsigned int GetElementPos(size_t a_Pos, string a_Name);
	size_t GetElementEndPos(size_t a_Pos, string a_Name);	
	void ReadRecordDef(size_t a_Pos);
	float GetTimeOffset();
	
};
