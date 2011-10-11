#pragma once
#include "IStringify.h"
#include "ISynchronizer.h"
#include <string>

using namespace std;

class RegistryEntry
{
private:
	string mValue;
	char buff[255];
	ISynchronizer* mSync;
public:
	RegistryEntry(ISynchronizer* aSync = NULL);
	RegistryEntry(string aValue, ISynchronizer* aSync = NULL);
	operator string() const;
	const RegistryEntry& operator=(string aValue);
	RegistryEntry(int aValue, ISynchronizer* aSync = NULL);
	operator int() const;
	const RegistryEntry& operator=(int aValue);
	RegistryEntry(float aValue, ISynchronizer* aSync = NULL);
	operator float() const;
	const RegistryEntry& operator=(float aValue);
	RegistryEntry(double aValue, ISynchronizer* aSync = NULL);
	operator double() const;
	const RegistryEntry& operator=(double aValue);
	RegistryEntry(bool aValue, ISynchronizer* aSync = NULL);
	operator bool() const;
	const RegistryEntry& operator=(bool aValue);
	RegistryEntry(IStringify& aValue, ISynchronizer* aSync = NULL);
	const RegistryEntry& operator=(IStringify& aValue);
	RegistryEntry(const RegistryEntry &r);
	const RegistryEntry& operator=(const RegistryEntry& right);
	void SetSynchronizer(ISynchronizer* aSync);

	RegistryEntry(unsigned int aValue, ISynchronizer* aSync = NULL);
	operator unsigned int() const;
	const RegistryEntry& operator=(unsigned int aValue);
	RegistryEntry(unsigned short aValue, ISynchronizer* aSync = NULL);
	operator unsigned short() const;
	const RegistryEntry& operator=(unsigned short aValue);
	~RegistryEntry(void);
};

