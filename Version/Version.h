#pragma once

namespace RepositoryCore
{
	class Version
	{
	public:
		Version() = default;
		Version& increment();
		Version& lock();

		Version& isAbleToLock() { ableToLock = true; return *this; }

	private:
		int versionNum_ = 1;
		bool ableToLock = false;
		bool isLocked = false;
	};
}