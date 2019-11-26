#include "Version.h"
#include <string>
#include <iostream>

using namespace RepoCore;

Version& Version::increment()
{
	if (!isLocked)
	{
		versionNum_++;
	}
	else
	{
		std::cout << "Version is locked, failed to update." << std::endl;
	}
	return *this;
}

Version& Version::lock()
{
	if (ableToLock)
	{
		isLocked = true;
	}
	else
	{
		std::cout << "Unable to lock, failed to lock verison." << std::endl;
	}
	return *this;
}

int main()
{
	return 0;
}