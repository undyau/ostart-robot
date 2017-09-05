/*
 * Chain.cxx
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

#include "Chain.hpp"
#include <iostream>
CChain::CChain(bool a_AnchoredStart) : m_AnchoredStart(a_AnchoredStart)
{
}

void CChain::AddRecord(std::unique_ptr<CRecord> a_Rec)
{
	m_Records.push_back(move(a_Rec));
}

std::list<CSlot> CChain::RecordsForTime(CTimeVal a_TimeVal)
{
	std::list<CSlot> list;
	for (auto& x: m_Records)
		{
		auto newRecs = x->RecordsForTime(a_TimeVal);
		list.insert(list.end(), newRecs.begin(), newRecs.end());
		}
		
	return list;
}

float CChain::DurationForTime(CTimeVal a_TimeVal)
{
	float res(0);
	for (auto& x: m_Records)
		res = res + x->DurationForTime(a_TimeVal);

	return res;
}
	
	
string CChain::RecordsForTimeAsString(CTimeVal a_TimeVal)
{
	string result;
	std::list<CSlot> list = RecordsForTime(a_TimeVal);
	for (auto x: list)
		result += x.m_FileName + " ";

	return result;
}


