/*
 * AnnouncementRecord.cxx
 * 
 * Copyright 2014  <pi@raspberrypi>
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
 
#include "AnnouncementRecord.hpp"
#include "SoundLibrary.hpp"

extern CSoundLibrary* gSoundLibrary;

CAnnouncementRecord::CAnnouncementRecord(string a_Name) : CRecord(a_Name)
  {
  }
 
float CAnnouncementRecord::DurationForTime(CTimeVal a_Time)
  {
  return gSoundLibrary->GetAnnouncementDuration(Name());
  }
	
std::list<CSlot> CAnnouncementRecord::RecordsForTime(CTimeVal a_Time)
  {
  CSlot slot;
  slot.m_Time = a_Time;
  slot.m_FileName = gSoundLibrary->GetAnnouncementFile(Name());
  std::list<CSlot> retVal;
  retVal.push_back(slot);
  return retVal;
  }


