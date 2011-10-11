/*
 * Registry.cpp
 *
 *  Created on: Aug 22, 2011
 *      Author: smarlin
 */

#include "Registry.h"
#include "MutexLock.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

Registry* Registry::mInst = NULL;

Registry* Registry::Instance()
{
	if(mInst == NULL)
	{
		mInst = new Registry;
	}
	return mInst;
}

Registry::Registry() :
	mut(WAIT_FOREVER),
	regEntryMut(WAIT_FOREVER)
{
}

Registry::~Registry()
{
	if(mInst != NULL)
	{
		delete mInst;
	}
	// Note:  mutexes will be destroyed after this point
}

RegistryEntry& Registry::operator[](string aKey)
{
	MutexLock m(&(this->mut));
	return mRegStorage[aKey];
}

bool Registry::KeyExists(string aKey)
{
	MutexLock m(&(this->mut));
	dictionaryTp::iterator i = mRegStorage.find(aKey);
	return (i!= mRegStorage.end());
}

bool Registry::readLine(ifstream& stream, string& aLine)
{
	const int kBuffSz = 4096;
	char buff[kBuffSz];
	stream.getline(buff,kBuffSz);
	if(stream.fail() || stream.bad())
	{
		return false;
	}
	aLine.assign(buff);
	aLine.assign(trim(aLine,' '));
	aLine.assign(trim(aLine,'\t'));
	aLine.assign(trim(aLine,'\r'));
	return true;
}

bool Registry::writeLine(ofstream& stream, string& aVarName, string&aVarVal)
{
	stream.write(aVarName.c_str(),aVarName.length());
	if(stream.fail())
	{
		return false;
	}
	stream.write("=",1);
	if(stream.fail())
	{
		return false;
	}
	stream.write(aVarVal.c_str(),aVarVal.length());
	if(stream.fail())
	{
		return false;
	}
	stream.write("\r\n",2);
	if(stream.fail())
	{
		return false;
	}
	return true;
}

string Registry::trim(string& line, char delimeter)
{
	string ret("");
	while(line.compare("") != 0)
	{
		ret.append(head(line,delimeter));
	}
	return ret;
}

string Registry::head(string& line, char delimeter)
{
	string ret("");
	int indx = line.find_first_of(delimeter);
	if(indx == (int)string::npos)
	{
		ret.assign(line);
		line.assign("");
		return ret;
	}
	if(indx != 0)
	{
		ret.assign(line.substr(0,indx));
	}
	else
	{
		ret.assign("");
	}
	if(((int)(line.length())) > indx + 1)
	{
		line.assign(line.substr(indx + 1, line.length() - indx - 1));
	}
	else
	{
		line.assign("");
	}
	return ret;
}


bool Registry::ReadFromFile(string aFileName)
{
	mFileName = aFileName;
	ifstream inFile(aFileName.c_str(),ios_base::in);
	if(!inFile)
	{
		return false;
	}
	string fileLine("");
	while(readLine(inFile,fileLine) == true)
	{
		if(fileLine.compare("") == 0)
		{
			continue;
		}
		string line = head(fileLine,'#');
		if(line.compare("") == 0)
		{
			continue;
		}
		string headStr = head(line,'=');
		if(headStr.compare("") == 0)
		{
			continue;
		}
		if(line.compare("") == 0)
		{
			continue;
		}
		(*this)[headStr]=line;
		line.assign("");
	}
	inFile.close();
	return true;
}

bool Registry::WriteToFile(string aFileName)
{
	MutexLock m(&(this->mut));
	string tempFileName = aFileName;
	tempFileName.append(".bak");
	ofstream outFile(tempFileName.c_str(),ios_base::out);
	if(!outFile)
	{
		return false;
	}
	for(dictionaryTp::iterator i = mRegStorage.begin(); i != mRegStorage.end(); i++)
	{
		string aName = i->first;
		string aVal = (string)(i->second);
		if(writeLine(outFile,aName,aVal) == false)
		{
			outFile.close();
			return false;
		}
	}
	outFile.close();
	const int kCmdBuffSz = 1024;
	char cmdBuff[kCmdBuffSz];
	sprintf(cmdBuff,"mv -f -T %s %s",tempFileName.c_str(),aFileName.c_str());
	if(system(cmdBuff) < 0)
	{
		return false;
	}
	return true;
}

void Registry::GetKeys(list<string>& aKeyList)
{
	aKeyList.clear();
	MutexLock m(&(this->mut));
	for(dictionaryTp::iterator i = mRegStorage.begin(); i != mRegStorage.end(); i++)
	{
		aKeyList.push_back(i->first);
	}
}

void Registry::Print()
{
	printf("**************Registry Dump**************\n");
	MutexLock m(&(this->mut));
	for(dictionaryTp::iterator i = mRegStorage.begin(); i != mRegStorage.end(); i++)
	{
		string val = (string)(i->second);
		printf("%s=%s\n",i->first.c_str(),val.c_str());
	}
	printf("**************Registry Dump End**********\n");
}

