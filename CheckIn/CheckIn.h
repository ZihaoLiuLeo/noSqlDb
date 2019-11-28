#pragma once
/////////////////////////////////////////////////////////////////////
// CheckIn.h - CheckIn mathods for repository                      //
// ver 1.0                                                         //
// Zihao Liu, fall 2019                                            //
/////////////////////////////////////////////////////////////////////


#include <string>
#include "../PayLoad/Payload.h"
#include "../RepositoryCore/RepositoryCore.h"
#include "../FileSystem/FileSystemDemo/FileSystem.h"
#include "../Version/Version.h"
#include <vector>

namespace RepoCore {
	using namespace FileSystem;

	template<typename P>
	class CheckIn
	{
		using FileName = std::string;
		using PackageName = std::string;
		using Status = bool;

	public:
		CheckIn() = default;
		CheckIn<P>(RepositoryCore<P>& rc) : rc_(rc) {};
		CheckIn(const CheckIn<P>& ci);
		CheckIn(CheckIn<P>&& ci);

		CheckIn<P>& operator=(const CheckIn<P>& ci);
		CheckIn<P>& operator=(CheckIn<P>&& ci);

		CheckIn<P>& addPackage(P p);

		CheckIn<P>& Close();

		bool ableToClose(); // find all dependent file for this check in

		P* payload() { return p; }
		P payload() const { return p; }
		void payload(const P& payload) { p = payload; }

	private:
		RepositoryCore<P>& rc_;
		P* p_ = nullptr;
		Status isOpen = true;
		Version version;	
	};

	template<typename P>
	CheckIn<P>::CheckIn(const CheckIn<P>& ci)
	{
		rc_ = ci.rc_;
	}

	template<typename P>
	CheckIn<P>::CheckIn(CheckIn<P>&& ci)
	{
		rc_ = ci.rc_;
	}

	template<typename P>
	CheckIn<P>& CheckIn<P>::operator=(const CheckIn<P>& ci)
	{
		if (this == &ci) return *this;
		rc_ = ci.rc_;
		return *this;
	}

	template<typename P>
	CheckIn<P>& CheckIn<P>::operator=(CheckIn<P>&& ci)
	{
		if (this == &ci) return *this;
		rc_ = ci.rc_;
		return *this;
	}

	template<typename P>
	CheckIn<P>& CheckIn<P>::addPackage(P p)
	{
		
		if(!isOpen())
		{
			version.increment();
		}

		p_ = &p;
		std::string packagePath = p.value();
		std::string repoPath = rc_.path();

		// store info in database
		P p_temp = p;
		p_temp.version(version.getNumInt());
		p_temp.name(repoPath);
		rc_.storePackage(p_temp);

		// go to checkin package location
		FileSystem::Directory::setCurrentDirectory(repoPath);
		std::vector<std::string> packageFiles = FileSystem::Directory::getFiles();
		

		for (size_t i = 0; i < packageFiles.size(); ++i)
		{
			// file info to be copied from
			std::string fs = FileSystem::Path::fileSpec(p.value(), packageFiles[i]);

			// file info to be copied to
			std::string tname = packageFiles[i] + "." + version.getNumString();
			std::string ts = FileSystem::Path::fileSpec(repoPath, tname);

			
			

			//copy file into repo path
			File to(ts);
			to.open(File::out, File::text);
			File from(fs);
			from.open(File::in);

			while (to.isGood() && from.isGood())
			{
				std::string temp = from.getLine();
				to.putLine(temp);
				to.putLine("\n");
			}
			to.close();
			from.close();
		}

		if (isOpen)
		{
			if (ableToClose())
			{
				version.isAbleToLock();
			}
		}
		
		return *this;
	}

	template<typename P>
	bool CheckIn<P>::ableToClose()
	{
		if (p_ != nullptr)
		{
			std::vector<std::string> depends;
			depends = p_->dependPacks();

			for (auto f : depends)
			{
				if (!rc_.containsPack(f))
				{
					return false;
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	template<typename P>
	CheckIn<P>& CheckIn<P>::Close()
	{
		version.lock();
		isOpen = false;
	}
}