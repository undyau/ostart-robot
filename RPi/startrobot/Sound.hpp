#include <string>

using namespace std;

class CSound 
{
public:
	CSound(string a_FileNames);
	void Play();

private:
	string m_FileNames;
	string EscapedFileNames();
	bool IsWavSuffix(string s);
};
