#include "RegistryEntry.h"
#include <stdlib.h>
#include <stdio.h>
#include "MutexLock.h"

RegistryEntry::~RegistryEntry(void)
{
}

RegistryEntry::RegistryEntry(ISynchronizer* aSync)
{
	mSync = aSync;
	mValue = "";
}

RegistryEntry::RegistryEntry(string aValue, ISynchronizer* aSync)
{
	mSync = aSync;
	mValue = aValue;
}

RegistryEntry::operator string() const
{
	if(mSync != NULL)
	{
		MutexLock(mSync->getMutexPtr());
		return mValue;
	}
	return mValue;
}

const RegistryEntry& RegistryEntry::operator=(string aValue)
{
	if(mSync != NULL)
	{
		MutexLock(mSync->getMutexPtr());
		mValue = aValue;
		return *this;
	}
	mValue = aValue;
	return *this;
}

RegistryEntry::RegistryEntry(unsigned short aValue, ISynchronizer* aSync)
{
	mSync = aSync;
	sprintf(buff,"%u",aValue);
	mValue.assign(buff);
}

RegistryEntry::operator unsigned short() const
{
	if(mSync != NULL)
	{
		MutexLock(mSync->getMutexPtr());
		return  (unsigned short) strtoul(mValue.c_str(), NULL, 10);
	}
	return  (unsigned short) strtoul(mValue.c_str(), NULL, 10);
}

const RegistryEntry& RegistryEntry::operator=(unsigned short aValue)
{
	if(mSync != NULL)
	{
		MutexLock(mSync->getMutexPtr());
		sprintf(buff,"%u",aValue);
		mValue.assign(buff);
		return *this;
	}
	sprintf(buff,"%u",aValue);
	mValue.assign(buff);
	return *this;
}

RegistryEntry::RegistryEntry(unsigned int aValue, ISynchronizer* aSync)
{
	mSync = aSync;
	sprintf(buff,"%u",aValue);
	mValue.assign(buff);
}

RegistryEntry::operator unsigned int() const
{
	if(mSync != NULL)
	{
		MutexLock(mSync->getMutexPtr());
		return  (unsigned int) strtoul(mValue.c_str(), NULL, 10);
	}
	return  (unsigned int) strtoul(mValue.c_str(), NULL, 10);
}

const RegistryEntry& RegistryEntry::operator=(unsigned int aValue)
{
	if(mSync != NULL)
	{
		MutexLock(mSync->getMutexPtr());
		sprintf(buff,"%u",aValue);
		mValue.assign(buff);
		return *this;
	}
	sprintf(buff,"%u",aValue);
	mValue.assign(buff);
	return *this;
}

RegistryEntry::RegistryEntry(int aValue, ISynchronizer* aSync)
{
	mSync = aSync;
	sprintf(buff,"%d",aValue);
	mValue.assign(buff);
}

RegistryEntry::operator int() const
{
	if(mSync != NULL)
	{
		MutexLock(mSync->getMutexPtr());
		return atoi(mValue.c_str());
	}
	return atoi(mValue.c_str());
}

const RegistryEntry& RegistryEntry::operator=(int aValue)
{
	if(mSync != NULL)
	{
		MutexLock(mSync->getMutexPtr());
		sprintf(buff,"%d",aValue);
		mValue.assign(buff);
		return *this;
	}
	sprintf(buff,"%d",aValue);
	mValue.assign(buff);
	return *this;
}

RegistryEntry::RegistryEntry(float aValue, ISynchronizer* aSync)
{
	mSync = aSync;
	sprintf(buff,"%f",aValue);
	mValue.assign(buff);
}

RegistryEntry::operator float() const
{
	if(mSync != NULL)
	{
		MutexLock(mSync->getMutexPtr());
		float ret = atof(mValue.c_str());
		return ret;
	}
	float ret = atof(mValue.c_str());
	return ret;
}

const RegistryEntry& RegistryEntry::operator=(float aValue)
{
	if(mSync != NULL)
	{
		MutexLock(mSync->getMutexPtr());
		sprintf(buff,"%f",aValue);
		mValue.assign(buff);
		return *this;
	}
	sprintf(buff,"%f",aValue);
	mValue.assign(buff);
	return *this;
}

RegistryEntry::RegistryEntry(double aValue, ISynchronizer* aSync)
{
	mSync = aSync;
	sprintf(buff,"%lf",aValue);
	mValue.assign(buff);
}

RegistryEntry::operator double() const
{
	if(mSync != NULL)
	{
		MutexLock(mSync->getMutexPtr());
		double ret = atof(mValue.c_str());
		return ret;
	}
	double ret = atof(mValue.c_str());
	return ret;
}

const RegistryEntry& RegistryEntry::operator=(double aValue)
{
	if(mSync != NULL)
	{
		MutexLock(mSync->getMutexPtr());
		sprintf(buff,"%lf",aValue);
		mValue.assign(buff);
		return *this;
	}
	sprintf(buff,"%lf",aValue);
	mValue.assign(buff);
	return *this;
}

RegistryEntry::RegistryEntry(bool aValue, ISynchronizer* aSync)
{
	mSync = aSync;
	(aValue == true) ?  mValue.assign("TRUE"):mValue.assign("FALSE");
}

RegistryEntry::operator bool() const
{
	if(mSync != NULL)
	{
		MutexLock(mSync->getMutexPtr());
		return (mValue.compare("TRUE")==0);
	}
	return (mValue.compare("TRUE")==0);
}

const RegistryEntry& RegistryEntry::operator=(bool aValue)
{
	if(mSync != NULL)
	{
		MutexLock(mSync->getMutexPtr());
		(aValue == true) ?  mValue.assign("TRUE"):mValue.assign("FALSE");
		return *this;
	}
	(aValue == true) ?  mValue.assign("TRUE"):mValue.assign("FALSE");
	return *this;
}

RegistryEntry::RegistryEntry(IStringify& aValue, ISynchronizer* aSync)
{
	mSync = aSync;
	mValue = aValue.ToString();
}

const RegistryEntry& RegistryEntry::operator=(IStringify& aValue)
{
	if(mSync != NULL)
	{
		MutexLock(mSync->getMutexPtr());
		mValue = aValue.ToString();
		return *this;
	}
	mValue = aValue.ToString();
	return *this;
}

RegistryEntry::RegistryEntry(const RegistryEntry &r)
{
	if(r.mSync != NULL)
	{
		mSync = r.mSync;
		mValue = r.mValue;
	}
	else
	{
		mSync = r.mSync;
		mValue = r.mValue;
	}
}

const RegistryEntry& RegistryEntry::operator=(const RegistryEntry& right)
{
	if(mSync != NULL)
	{
		MutexLock(mSync->getMutexPtr());
		mValue = right.mValue;
		return *this;
	}
	mValue = right.mValue;
	return *this;
}

void RegistryEntry::SetSynchronizer(ISynchronizer* aSync)
{
	mSync = aSync;
}
