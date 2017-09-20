/*
 * Sound.cxx
 *
 * Copyright 2016  Andy Simpson
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


#include "Sound.hpp"
#include <iostream>
#include <stdlib.h>

CSound::CSound(string a_FileNames) : m_FileNames(a_FileNames)
{

}

void CSound::Play()
{
	string cmd("aplay " + EscapedFileNames());
	cout << "Command is " << cmd << endl;
	system(cmd.c_str());
}

string CSound::EscapedFileNames()
{
	string retVal;

	// Should be list of .wav files, but some may contain spaces in their names
	//   Do progressive parse, noting when at file suffix
	//   Replace any spaces not following suffix with escape sequence

	string last4Characters("");

	while (m_FileNames.size() && m_FileNames.back() == ' ')
		m_FileNames.erase(m_FileNames.size()-1, 1);

	for (unsigned int i = 0; i < m_FileNames.size(); i++)
		if (!IsWavSuffix(last4Characters) && m_FileNames.at(i) == ' ')
			{
			retVal += "\\ ";
			if (last4Characters.size() >= 4)
				last4Characters.erase(0,1);
			last4Characters += ' ';
			}
		else
			{
			retVal += m_FileNames.at(i);
			if (last4Characters.size() >= 4)
				last4Characters.erase(0,1);
			last4Characters += m_FileNames.at(i);
			}
	return retVal;
}

bool CSound::IsWavSuffix(string s)
{
		if (s.size() != 4)
			return false;
		if (s.at(0) != '.')
			return false;
		if (s.at(1) != 'w' && s.at(1) != 'W')
			return false;
		if (s.at(2) != 'a' && s.at(2) != 'A')
			return false;	
		if (s.at(3) != 'v' && s.at(3) != 'V')
			return false;	
		return true;
}
