#include "HighTime.h"

CString Word(const CString a_Text, const unsigned int a_Index=0, const char a_Delim=' ', const CString a_DefaultValue="");
CHighTimeSpan FloatSecsToSpan(float a_Secs);
CHighTimeSpan HHMMSST2Span(CString a_Time);
CHighTimeSpan MM2Span(CString a_Time);
bool ToCHighTime(CString a_XmlTime, CHighTime& a_OutputTime);
float SpanToFloatSecs(CHighTimeSpan& a_Span);
CString HighTimeAsStr(CHighTime const & a_Time);
bool FileExists(CString a_File);
bool CopyAppendWaveFile(CString a_Source, CString a_Target);

CString ToXML(CString a_Label, CString& a_Value);
CString ToXML(CString a_Label, bool a_Value);
CString ToXML(CString a_Label, char* a_Value);
CString ToXML(CString a_Label, int a_Value);

void GetXMLValue(CString& a_SomeXML, CString a_Label, CString& a_Variable, CString a_Default = CString(""));
void GetXMLValue(CString& a_SomeXML, CString a_Label, bool& a_Variable, bool a_Default = false);
void GetXMLValue(CString& a_SomeXML, CString a_Label, int& a_Variable, int a_Default = 0);

void AFXAPI DDV_PCFileName(CDataExchange* pDX, int nIDC, const CString &a_Value);