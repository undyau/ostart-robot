/*
 * CSchedule.cxx
 *
 * Copyright 2016 Andy Simpson
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 * 
 */

#include "Schedule.hpp"
#include <string>
#include <fstream>
#include <unistd.h>
#include "AnnouncementRecord.hpp"
#include "TimeRecord.hpp"
#include "Sound.hpp"
#include <streambuf>
#include <math.h>
#include <iostream>

CSchedule::CSchedule(string a_Dir) : m_Dir(a_Dir)
{
}

bool CSchedule::Load()
{
	// Find the XML file 
	string fileName(m_Dir);
	fileName += "/forAndy.xml";

	ifstream t(fileName);
	if (!t.is_open())
	{
		printf("%s not opened\n", fileName.c_str());
		return false;
	}
	string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	m_XmlStr = str;

	// Find the Startlist file, if present
	string m_StartListFile = GetElementVal(0, "m_StartListFile");

	// Get frequency  (regex seems broken in this compiler version :(    )
	string val = GetElementVal(0, "m_Frequency");
	while (val.size() > 0 && val.substr(0,1) == " ")
		val = val.substr(1, val.size() -1);

	if (val.size() < 4 || val.size() > 5 || val.find(":", 0) < 1)
		return false;
	size_t pos = val.find(":", 0);
	m_FrequencySecs = atoi(val.substr(pos + 1, val.size() - (pos + 1)).c_str());
	m_FrequencySecs += 60 * (atoi(val.substr(0, pos).c_str()));

	// Get Records
	pos = GetElementPos(0, "Records");
	while ((pos = GetElementPos(pos, "Record"))!= string::npos)
		ReadRecordDef(pos++);
	return true;
}

size_t CSchedule::GetElementEndPos(size_t a_Pos, string a_Name)
{
	return m_XmlStr.find("</" + a_Name + ">", a_Pos);
}

unsigned int CSchedule::GetElementPos(size_t a_Pos, string a_Name)
{
	return m_XmlStr.find("<" + a_Name + ">", a_Pos);
}

string CSchedule::GetElementVal(size_t a_Pos, string a_Name)
{
	size_t start = GetElementPos(a_Pos, a_Name);
	if (start == string::npos)
		return string();

	size_t end = GetElementEndPos(a_Pos, a_Name);
	if (end == string::npos)
		return string();

	string retVal = m_XmlStr.substr(start + a_Name.size() + 2, end - (start + a_Name.size() + 2));

	return retVal;
}

int CSchedule::GetElementValInt(size_t a_Pos, string a_Name)
{
	string t = GetElementVal(a_Pos, a_Name);
	int tf = atoi(t.c_str());
	return tf;
}

bool CSchedule::GetElementValBool(size_t a_Pos, string a_Name)
{
	return !!GetElementValInt(a_Pos, a_Name);
}

float CSchedule::GetElementValTimeVal(size_t a_Pos, string a_Name)
{
	string txt = GetElementVal(a_Pos, a_Name);

	size_t colon = txt.find(":", 0);
	size_t point = txt.find(".", 0);
	
	if (colon > 0 && point == colon + 3 && point == txt.size() -2)
		{
		float retVal = atoi(txt.substr(colon + 1, 2).c_str());
		retVal += 60 * (atoi(txt.substr(0,colon).c_str()));
		retVal += (atoi(txt.substr(point+1, 1).c_str()))/10;			
		return retVal;	
		}		
	else
		return 0;	
}

void CSchedule::ReadRecordDef(unsigned int a_Pos)
{
	string name = GetElementVal(a_Pos, "m_Name");  // Name - will be file name for announcement type
	string type = GetElementVal(a_Pos, "Type"); //"Custom" or "Timed" or "StartList"
	string follow = GetElementVal(a_Pos, "m_FollowMsgName"); //Name of msg this follows (may be redundant)
	float playattime = GetElementValTimeVal(a_Pos, "m_TimingTime"); //Offset to start playing at (first record in chain only)
	bool finishattime = GetElementValBool(a_Pos, "m_FinishAtTime"); //True if this message finishes at a specific time
	bool aftermsg = GetElementValBool(a_Pos, "m_AfterMsg"); // True if this message comes after another
	int offsettime = GetElementValInt(a_Pos, "m_OffsetTime");  // announcement offset minutes , can be -ve	
	unique_ptr<CRecord> rec;
	static shared_ptr<CChain> lastChain = nullptr;
	
	if (type == "Custom")
	{
		rec = move(unique_ptr<CRecord>(new CAnnouncementRecord(name)));
    }
	else if (type = "StartList")
	{
        rec = move(unique_ptr<CRecord>(new CStartListRecord(name, offsettime)));    
    }
    else
    {
		if (type != "Timed")
		{
			cout << "Record " << name << " with unknown type " << type << " found - ignored" << endl;
			return;
		}
		rec = move(unique_ptr<CRecord>(new CTimeRecord(name, offsettime)));
    }
    
	if (aftermsg)
    {
    // Find right chain and insert it
    // Assume that last chain (if it exists) is the one we want
		if (!lastChain)
			return;   // logic error
		else
			lastChain->AddRecord(move(rec));
    }
	else
    {
		lastChain = shared_ptr<CChain>(new CChain(!finishattime));
		m_Program[playattime] = lastChain;
		m_Program[playattime]->AddRecord(move(rec));
    }
}

float CSchedule::GetTimeOffset()
{
	time_t rawtime;
	struct tm * timeinfo;

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	int secs = timeinfo->tm_sec + 60 * timeinfo->tm_min;
	secs %= m_FrequencySecs;
cout << "GetTimeOffset: " << timeinfo->tm_hour << ":" << timeinfo->tm_min << ":" << timeinfo->tm_sec << " " << secs << endl;	
	return float(secs);
}


void CSchedule::PlayNextSound()
{
// Get current base time
	float offset = GetTimeOffset();  // how far into period we are
	timeval basetv;
	gettimeofday(&basetv, NULL);
	basetv.tv_sec -= (int)offset;
	basetv.tv_usec = 0;
	CTimeVal baseTime(basetv);  // base time for the period
cout << "Base time is " << baseTime.TimeString() << endl;
	CTimeVal baseAnnounceTime(baseTime);
	baseAnnounceTime.AddSecs(m_FrequencySecs);
	float startSearchTime;
	bool nextInterval = false;
	map<float, shared_ptr<CChain>>::iterator i;

	for (i = m_Program.begin(); i != m_Program.end(); i++)  // should be retrieved in offset order
		cout << "Found chain with offset " << i->first << endl;
retry:
	startSearchTime = offset;

cout << endl;
cout << "Looking for next recording to play, basetime is: " << baseTime.TimeString() << " offset " << offset << endl;

// Find next chain

	for (i = m_Program.begin(); i != m_Program.end() && !(startSearchTime <= i->first); i++)  // should be retrieved in offset order
		;

	if (i != m_Program.end() && !i->second->AnchoredStart() &&
		i->second->DurationForTime(baseAnnounceTime) > offset)  // should be playing now, skip to next 
	{
		startSearchTime += i->second->DurationForTime(baseAnnounceTime);
		for (i = m_Program.begin(); i != m_Program.end() && !(startSearchTime <= i->first); i++)  // should be retrieved in offset order
			;
	}

	if (i == m_Program.end() && !nextInterval)  // wrap around
		{
		cout << "wrapping to next interval" << endl;
		offset = 0;
		nextInterval = true;
		baseTime.AddSecs(m_FrequencySecs);
		baseAnnounceTime.AddSecs(m_FrequencySecs);
		goto retry;
		}

	if (i == m_Program.end())
		{
		cout << "No sound to play :(" << endl;
		return;
		}

// Calculate the start time
// base time + offset of chain - duration if its anchored at end
	CTimeVal startTime(baseTime);
cout << "startTime was " << startTime.TimeString() << " adding on " << i->first << " secs" << endl;
	startTime.AddSecs(i->first);
cout << "startTime was " << startTime.TimeString() << " looking at Anchored Start: " << i->second->AnchoredStart() << endl;
	if (!i->second->AnchoredStart())
		startTime.AddSecs(- i->second->DurationForTime(baseAnnounceTime));

	// Get the list of file names as a string for aplay
	string files = i->second->RecordsForTimeAsString(baseAnnounceTime);
	CSound nextSound(files);
cout << "Chosen chain with offset " << i->first << " start time is " << startTime.TimeString() << endl;
CTimeVal now;
cout << "startTime now " << startTime.TimeString() << " at time " << now.TimeString() << endl;	
	// If startTime is later than now, wait for the right time to play the chain
	if (now < startTime)
		{
		float diff = startTime - CTimeVal();
		cout << "sleeping for " << diff << "seconds" << endl;
		usleep(diff*1000000);
		CTimeVal woken;
		cout << "woken at " << woken.TimeString() << endl;
		}

	nextSound.Play();
}

