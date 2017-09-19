/*
 * SoundLibrary.cxx
 * 
 * Copyright 2017  Andy Simpson
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

#include <ftw.h>
#include <fnmatch.h>
#include <libgen.h>
#include <string.h>
#include <iostream>
#include "Wave.hpp"
#include "SoundLibrary.hpp"
//#include <regex>  seems broken

extern CSoundLibrary* gSoundLibrary;

// Callback function used when traversing the folder of wav files 
static int callback(const char* fpath, const struct stat *sb, int typeflag)
{
	if (typeflag == FTW_F)
		{
		if (fnmatch("*.wav", fpath, FNM_CASEFOLD) == 0)
			{
			gSoundLibrary->ProcessFile(string(fpath));
			}			
		}
	return 0;
}

CSoundLibrary::CSoundLibrary(string a_Dir): m_StartList(nullptr))
{
	// Find every wav file under directory
	gSoundLibrary = this;
}

CSoundLibrary::~CSoundLibrary()
{
	gSoundLibrary = nullptr;
}

void CSoundLibrary::Init(CStartList* a_StartList)
{
	ftw(a_Dir.c_str(), callback, 16);    
}

void CSoundLibrary::ProcessFile(string a_File)
{
	// Extract name
	if (a_File.size() == 0)
		return;

	string name;
	char buf[255];
	strcpy(buf, a_File.c_str());
	name = basename(buf);
	if (name.size() < 5)
		return;
	name = name.substr(0, name.size() -4); // strip ".wav"

	// Try to get duration
	float duration = GetDuration(a_File);
	if (duration <= 0)
		return;
		
	if (IsNameFile(a_File))
	    {
	    m_NameSndDurations[name] = duration;
		m_NameSndFiles[name] = a_File;   
        }
    else if (IsTimeFile(name))
		{
		m_TimeSndDurations[name] = duration;
		m_TimeSndFiles[name] = a_File;
		}
	else
		{
		m_AnnouncementDurations[name] = duration;
		m_AnnouncementFiles[name] = a_File;
		}
} 


bool CSoundLibrary::IsNameFile(string a_File)
{
    return m_StartList && m_StartList->IsNameFile(a_File);
}


bool CSoundLibrary::IsTimeFile(string a_File)
{
//	regex re("^cd[hour|num][0-9]+$");
	if (a_File.size() < 6 || a_File.substr(0,2) != "cd")
		return false;
	if (a_File.substr(2,4) != "hour" && a_File.substr(2,3) != "num")
		return false;
	return true;
//	return regex_match(a_File, re);
}

float CSoundLibrary::GetDuration(string a_File)
{
	CWave wav(a_File);
	if (wav.isValid())
		return wav.getDuration();
	else
		return 0;
}


string CSoundLibrary::GetAnnouncementFile(string a_Title)
{
	if (m_AnnouncementFiles.find(a_Title) != m_AnnouncementFiles.end())
		return m_AnnouncementFiles[a_Title];
	else 
		return string();
}

float CSoundLibrary::GetAnnouncementDuration(string a_Title)
{
	if (m_AnnouncementDurations.find(a_Title) != m_AnnouncementDurations.end())
		return m_AnnouncementDurations[a_Title];
	else 
		return 0;	
}

std::string CSoundLibrary::GetTimeSndFile(string a_Title)
{
	if (m_TimeSndFiles.find(a_Title) != m_TimeSndFiles.end())
		return m_TimeSndFiles[a_Title];
	else 
		return string();
}

float CSoundLibrary::GetTimeSndDuration(string a_Title)
{
	if (m_TimeSndDurations.find(a_Title) != m_TimeSndDurations.end())
		return m_TimeSndDurations[a_Title];
	else 
		return 0;	
} 

std::string CSoundLibrary::GetNameSndFile(string a_Title)
{
	if (m_NameSndFiles.find(a_Title) != m_NameSndFiles.end())
		return m_NameSndFiles[a_Title];
	else 
		return string();
}

float CSoundLibrary::GetNameSndDuration(string a_Title)
{
	if (m_NameSndDurations.find(a_Title) != m_NameSndDurations.end())
		return m_NameSndDurations[a_Title];
	else 
		return 0;	
} 
