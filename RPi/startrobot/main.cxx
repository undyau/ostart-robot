/*
 * main.cxx
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


#include <iostream>
#include "Sound.hpp"
#include "Schedule.hpp"
#include "SoundLibrary.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <streambuf>
#include <sstream>
#include <fstream>

using namespace std;

CSoundLibrary* gSoundLibrary;


bool FindDirOnMount(string a_Mount, string& a_Dir)
{
	DIR *dp;
	struct dirent* ep;
	
	dp = opendir(a_Mount.c_str());
	if (dp == NULL)
		return false;
	
	while ((ep = readdir (dp)) != NULL)
	{
		if (ep->d_type == DT_DIR && ep->d_name[0] != '.')
			{
				string candidate(ep->d_name);
				candidate = a_Mount + "/" + candidate;
				string candidateFile(candidate);
				candidateFile  += "/forAndy.xml";
				
				struct stat buffer;
				if (stat(candidateFile.c_str(), &buffer) == 0)
					{
						a_Dir = candidate;
						return true;
					}
			}
	}
	return false;
}

bool FindDir(string& a_Dir)
{
	// Search through all plug-in media and find one with a top level 
	// directory containing file "forAndy.xml"
	
	string fileName("/proc/mounts");
retry:
	{
		ifstream t(fileName);
		if (!t.is_open())
		{
			printf("%s not opened\n", fileName.c_str());
			return false;
		}
			
		string line;
		while (getline(t, line))
		{
			istringstream iss(line);
			string device, mount;
			if (!(iss >> device >> mount))
				continue;  //garbage line ?
			if (device.substr(0, 7) == "/dev/sd")  //candidate
				if (FindDirOnMount(mount, a_Dir))
					return true;
		}
	}
	cout << "Didn't find candidate directory, sleeping for 10 seconds" << endl;
	usleep(10000000);
	goto retry;
	
	return false;	
}

int main(int argc, char **argv)
{
	cout<<"Start Robot is initialising" << endl;
	string dir;
	if (!FindDir(dir))
		return -1;

	CSoundLibrary lib(dir);
	gSoundLibrary = &lib;
	CSchedule schedule(dir);
	if (schedule.Load())
		{
		cout << "Schedule loaded" << endl;

		while (1) // loop forever
			{
			schedule.PlayNextSound();
			}
		}
	return 0;
}

