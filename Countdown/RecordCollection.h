// RecordCollection.h: interface for the CRecordCollection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECORDCOLLECTION_H__2C421587_18A8_4EFA_8868_6AD1AD8E0E94__INCLUDED_)
#define AFX_RECORDCOLLECTION_H__2C421587_18A8_4EFA_8868_6AD1AD8E0E94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRecording;
class CProgressWnd;
#include <map>
#include "HighTime.h"

class CRecordCollection  
{
public:
	CRecordCollection();
	virtual ~CRecordCollection();
    CRecording* AddCustomRecord(CString& a_Name, CString& a_FollowMsgName);
    CRecording* AddCustomRecord(CString& a_Name, bool a_FinishAtTime, CString a_TimingTime);
    CRecording* AddTimeRecord(bool a_FinishAtTime, CString a_TimingTime, CString a_OffsetTime);
    CRecording* AddTimeRecord(CString& a_FollowMsgName, CString a_OffsetTime);

    bool ReloadFromXML(CString a_XMLFileName);
    bool SaveToXML(CString a_XMLFileName, CString a_StartTime = "14:00", CString a_StopTime = "14:05");
    CString GetFileName() {return m_FileName;};
    void SetFileName(CString a_FileName) {m_FileName = a_FileName;};

    CString LastError();
    void Clear();
    bool RecordExists(CString a_Name);
    //bool CheckSchedule(CString& a_First, CString& a_Last);
	bool DeleteRecord(CString& a_Name);
    CRecording* GetRec(int a_Index);
	CRecording* GetRec(CString a_Name);
    int Count() const { return m_Map.size();};

    bool MakeRecording(CString a_First, CString& a_Last, CString& a_Every, CString a_FileName);
    bool ConvertToMp3(CString a_FileName);

    void SetProgressWnd(CProgressWnd* a_Progress);
    void ClearProgressWnd();
    void SetProgressWndRange(int a_Lower, int a_Upper);
    void SetProgressWndText(CString a_Caption, LPCTSTR a_Fmt, ...);
    void StepProgressWnd();
    bool ProgressWndCancelled();
	CString StartTime() {return m_StartTime;};
	CString StopTime() {return m_StopTime;};
	void StartTime(CString a_StartTime) {m_StartTime = a_StartTime;};
	void StopTime(CString a_StopTime) {m_StopTime = a_StopTime;};
	CString Frequency() {return m_Frequency;};
	void Frequency(CString a_Frequency) {m_Frequency = a_Frequency;};
    CRecording** GetSortedRecords();
	bool HasDependent(CString& a_Name);
	bool ModifyRecord(CString& a_Name,
					 bool a_IsCustom,
					 bool a_IsFollow,
					 CString& a_TimingTime,
					 bool a_FinishAtTime,
					 CString& a_FollowMsgName,
					 CString& a_OffsetTime);


private:
    CString m_LastError;
	int m_Version;
    int m_TimeRecordsCreated;
    typedef std::map<CString, CRecording*> t_MapType;
    t_MapType m_Map;
    CProgressWnd* m_Progress;
    CString m_FileName;

    CHighTime OffsetStartTime(CRecording* a_Rec, CHighTime a_Time); 
    bool Follows(CRecording* a_Recording, CRecording* a_PossiblePredecessor);
    CString GapFileName();    
    bool IsBefore(CRecording& a_Lhs, CRecording& a_Rhs);
	bool TimingTimeExists(CString a_TimingTime);

	CString m_StartTime;
	CString m_StopTime;
	CString m_Frequency;

};

#endif // !defined(AFX_RECORDCOLLECTION_H__2C421587_18A8_4EFA_8868_6AD1AD8E0E94__INCLUDED_)
