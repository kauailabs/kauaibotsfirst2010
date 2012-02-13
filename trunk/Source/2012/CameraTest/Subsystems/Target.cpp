#include "Target.h"

bool operator<(const Target& left, const Target& right)
{
	return left.TotalScore() < right.TotalScore();
}

