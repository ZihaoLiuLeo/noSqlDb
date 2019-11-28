#pragma once
/////////////////////////////////////////////////////////////////////
// Version.h - Version monitoring mathods for repository           //
// ver 1.0                                                         //
// Zihao Liu, fall 2019                                            //
/////////////////////////////////////////////////////////////////////
#include <string>

namespace RepoCore
{
	class Version
	{
	public:
		Version() = default;
		Version& increment();
		Version& lock();

		void isAbleToLock() { ableToLock = true; }

		bool AbleToLock() const { return ableToLock; }

		std::string getNumString() const { return std::to_string(versionNum_); }

		int getNumInt() const { return versionNum_; }

	private:
		int versionNum_ = 1;
		bool ableToLock = false;
		bool isLocked = false;
	};
}