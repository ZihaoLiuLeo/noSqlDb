#pragma once
/////////////////////////////////////////////////////////////////////
// CheckIn.h - CheckIn mathods for repository                      //
// ver 1.0                                                         //
// Zihao Liu, Spring 2018                                          //
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

		CheckIn<P>& addFile(P p); // open checkin then store info in db, store file in repo, add version to filename, close then update version
		CheckIn<P>& addPackage(P p);

		CheckIn<P>& Close();

		bool ableToClose(); // find all dependent file for this check in

		P* payload() { return p; }
		P payload() const { return p; }
		void payload(const P& payload) { p = payload; }

	private:
		RepositoryCore<P>& rc_;
		P p_;
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
	CheckIn<P>& CheckIn<P>::addFile(P p)
	{
		rc_.storeFile(p);
		if (isOpen)
		{
			
			//TODO denpendency check
			// p.dependFile all in dir
			/*
			open p.value();
			new filename: filename + version.num(), copy content
			add to rc_.path();
			*/
		}
		return *this;
	}

	template<typename P>
	CheckIn<P>& CheckIn<P>::addPackage(P p)
	{
		
		if(!isOpen())
		{
			version.increment();
		}

		p_ = p;
		std::string packagePath = p.value();
		std::string repoPath = rc_.path();

		// go to checkin package location
		FileSystem::Directory::setCurrentDirectory(repoPath);
		std::vector<std::string> packageFiles = FileSystem::Directory::getFiles();
		

		for (size_t i = 0; i < packageFiles.size(); ++i)
		{
			// file info to be copied from
			std::string fs = FileSystem::Path::fileSpec(p.value(), packageFiles[i]);

			// file info to be copied to
			std::string tname = packageFiles[i] + "." + version.getNum();
			std::string ts = FileSystem::Path::fileSpec(repoPath, tname);

			// store info in database
			P p_temp = p;
			p_temp.value(packageFiles[i]);
			rc_.storeFile(p_temp);
			

			//copy file into repo path
			File to(ts);
			to.open(File::out, File::text);
			File from(fs);
			from.open(File::in, File::text);

			while (to.isGood() && from.isGood())
			{
				std::string temp = from.getLine();
				to.putLine(temp);
				to.putLine("\n");
			}
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
		std::vector<std::string> depends;
		depends = p_.dependFiles();

		std::string repoPath = rc_.path();
		FileSystem::Directory::setCurrentDirectory(repoPath);
		std::vector<std::string> pf = FileSystem::Directory::getFiles();

		for (auto f : depends)
		{
			if (std::find(pf.begin(), pf.end(), f) == pf.end())
			{
				return false;
			}
		}
		return true;
	}

	template<typename P>
	CheckIn<P>& CheckIn<P>::Close()
	{
		version.lock();
		isOpen = false;
	}
}