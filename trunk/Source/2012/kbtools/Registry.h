/*
 * Registry.h
 *
 *  Created on: Aug 22, 2011
 *      Author: smarlin
 */

#ifndef REGISTRY_H_
#define REGISTRY_H_
#include <map>
#include <string>
#include "RegistryEntry.h"
#include <exception>
#include "ISynchronizer.h"
#include <list>

using namespace std;

class Registry : public ISynchronizer
{
private:
	typedef map<string,RegistryEntry> dictionaryTp;
	dictionaryTp mRegStorage;
	static Registry* mInst;
	Mutex mut;
	Mutex regEntryMut;
	string mFileName;

	Registry();
	string head(string& line, char delimeter);
	string trim(string& line, char delimeter);
	bool readLine(ifstream& stream, string& aLine);
	bool writeLine(ofstream& stream, string& aVarName, string&aVarVal);
public:
	static Registry* Instance();
	bool ReadFromFile(string aFileName);
	string GetFileName(){ return mFileName; };
	bool WriteToFile(string aFileName);
	RegistryEntry& operator[](string aKey);
	bool KeyExists(string aKey);
	virtual Mutex* getMutexPtr() { return &regEntryMut; };
	virtual ~Registry();
	void GetKeys(list<string>& aKeyList);
	void Print();
};

#endif /* REGISTRY_H_ */
