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

std::list<string> CStartListRecord::TitlesForTime(CTimeVal a_Time)
  {
  std::list<string> retVal;
  
  int hour, min;
  char buf[50];
  
 // Note: the time corresponds to the next start time 
 // need to add on the duration of the interval ? 
  
  hour = a_Time.Hour();
  min = a_Time.Min();
  
  min = (60 * hour) + min + m_MinOffset;
  if (min < 0)
    min += 24*60;
  hour = min / 60;
  min = min % 60;
  
  if (min == 0)
      {
      sprintf(buf, "cdhour%02d", hour);
      retVal.push_back(string(buf));
      return retVal;
      }

    if (hour > 0 && hour < 10)
        sprintf(buf, "cdnum%01d", hour);
    else
        sprintf(buf, "cdnum%02d", hour);
    retVal.push_back(string(buf));
    
    sprintf(buf, "cdnum%02d", min); 
    retVal.push_back(string(buf));    
    return retVal;
}
  
float CStartListRecord::DurationForTime(CTimeVal a_Time)
  {
  std::list<string> list = TitlesForTime(a_Time);
  float retVal(0);
  for (auto i: list)
      retVal += gSoundLibrary->GetTimeSndDuration(i);
      
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
      slot.m_FileName = gSoundLibrary->GetTimeSndFile(i);
      duration += gSoundLibrary->GetTimeSndDuration(i);
      retVal.push_back(slot);
      }
      
  return retVal;
  }
	
