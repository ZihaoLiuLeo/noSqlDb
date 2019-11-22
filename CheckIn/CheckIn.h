#pragma once
/////////////////////////////////////////////////////////////////////
// CheckIn.h - CheckIn mathods for repository                      //
// ver 1.0                                                         //
// Zihao Liu, Spring 2018                                          //
/////////////////////////////////////////////////////////////////////


#include <string>
#include "../PayLoad/Payload.h"
#include "../RepositoryCore/RepositoryCore.h"
#include "../Version/Version.h"
#include "../FileSystem/FileSystemDemo/FileSystem.h"

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

		bool ableToSetStatus(P p); // find all dependent file for this check in

		P* payload() { return p; }
		P payload() const { return p; }
		void payload(const P& payload) { p = payload; }

	private:
		RepositoryCore<P>& rc_;
		P p;
		Status isOpen = true;
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
		/*std::string packagePath = p.value();
		std::vector<std::string> packageFiles = FileSystem::Directory::getFiles();
		for (size_t i = 0; i < packageFiles.size(); ++i)
			rc_.store(packageFiles[i]);*/
		if (isOpen)
		{
			//for all file in p.value
			//	rc_.store(p);
			/*
			open p.value();
			new filename: filename + version.num(), copy content
			add to rc_.path();
			*/
		}
		else
		{
			/*
			same process,
			update version
			*/
		}
		return *this;
	}

	template<typename P>
	bool CheckIn<P>::ableToSetStatus()
	{
		/*
		for f in p.dependFiles
			if current dir contains f

			else 
				return false
		
		return true;
		*/
	}
}