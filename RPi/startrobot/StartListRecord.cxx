/*
 * StartListRecord.cxx
 * 
 * Copyright 2017  <pi@raspberrypi>
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
 
#include "StartListRecord.hpp"
#include "SoundLibrary.hpp"

extern CSoundLibrary* gSoundLibrary;

CStartListRecord::CStartListRecord(string a_Name, int a_MinOffset) : CRecord(a_Name)
  {
  m_MinOffset = a_MinOffset;
  }

std::list<string> CStartList::TitlesForTime(CTimeVal a_Time)
	{
	std::vector<string> result;
	std::pair <std::multimap<CTimeVal, string>::iterator, std::multimap<CTimeVal, string>::iterator> ret;
	ret = m_StartTimes.equal_range(a_Time);
	for (std::multimap<CTimeVal, string>::iterator it = ret.first; it != ret.second; ++it)
		result.push_back(it->second);
	return result;
	}

float CStartListRecord::DurationForTime(CTimeVal a_Time)
  {
  std::list<string> list = TitlesForTime(a_Time);
  float retVal(0);
  for (auto i: list)
      retVal += gSoundLibrary->GetNameSndDuration(i);
      
  return retVal;
  }
  
std::list<CSlot> CStartListRecord::RecordsForTime(CTimeVal a_Time)
  {
  std::list<string> list = TitlesForTime(a_Time);
  std::list<CSlot> retVal;
  float duration(0);
  for (auto i: list)
      {
      CSlot slot;
      slot.m_Time.AddSecs(duration);
      slot.m_FileName = gSoundLibrary->GetNameSndFile(i);
      duration += gSoundLibrary->GetNameSndDuration(i);
      retVal.push_back(slot);
      }
      
  return retVal;
  }
	
