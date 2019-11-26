#pragma once
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

		std::string getNum() const { return std::to_string(versionNum_); }

	private:
		int versionNum_ = 1;
		bool ableToLock = false;
		bool isLocked = false;
	};
}