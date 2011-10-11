#pragma once
#include <string>

using namespace std;

class IStringify
{
public:
	virtual string ToString() = 0;
	virtual void FromString(string aValue) = 0;
	const IStringify &operator=(string aValue) { FromString(aValue);  return *this;};
};

