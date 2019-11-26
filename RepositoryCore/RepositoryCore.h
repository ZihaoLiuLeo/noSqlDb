#pragma once
/////////////////////////////////////////////////////////////////////
// RepositoryCore.h - Implements RepositoryCore for file managing  //
// ver 1.0                                                         //
// Zihao Liu, Spring 2018                                          //
/////////////////////////////////////////////////////////////////////

#include "../DbCore/DbCore.h"
#include "../Query/Query.h"
#include "../PayLoad/Payload.h"
#include <string>
#include "../FileSystem/FileSystemDemo/FileSystem.h"


namespace RepoCore
{
	using namespace NoSqlDb;
	using namespace FileSystem;
	
	template<typename P>
	class RepositoryCore
	{
	public:
		using FilePath = std::string;
		using Key = std::string;
		using Keys = std::vector<Key>;

		RepositoryCore() = default;
		RepositoryCore(const RepositoryCore<P>& rc);
		RepositoryCore(RepositoryCore<P>&& rc);

		RepositoryCore<P>& operator=(const RepositoryCore<P>& q);
		RepositoryCore<P>& operator=(RepositoryCore<P> && q);

		FilePath& path() { return path_; }
		FilePath path() const { return path_; }
		void path(FilePath& path) { path = path_ };

		bool createRepoDir();

		RepositoryCore<P>& storeFile(P p);

	private:
		DbCore<P> db_;
		FilePath path_ = "";
		//DbElement<P>* p_dbe = nullptr;
	};

	//----< copy constructor >--------------------------------------------
	template<typename P>
	RepositoryCore<T>::RepositoryCore(const RepositoryCore<T>& rc)
	{
		p_db = rc.p_db;
	}

	//----< move constructor >--------------------------------------------
	template<typename P>
	RepositoryCore<T>::RepositoryCore(RepositoryCore<T>&& rc)
	{
		p_db = rc.p_db;
	}

	//----< copy assignment >--------------------------------------------
	template<typename P>
	RepositoryCore<T>& RepositoryCore<T>::operator=(const RepositoryCore<T>& rc)
	{
		if (this == &q) return *this;
		p_db = rc.p_db;
		return *this;
	}

	//----< move assignment >--------------------------------------------
	template<typename P>
	RepositoryCore<T>& RepositoryCore<T>::operator=(RepositoryCore<T>&& rc)
	{
		if (this == &q) return *this;
		p_db = rc.p_db;
		return *this;
	}

	template<typename P>
	RepositoryCore<P>& RepositoryCore<P>::storeFile(P p)
	{
		Key& key = p.value();
		DbElement<P>& dbe = db_[key];
		dbe.children(p.dependFiles);
		dbe.name(FileSystem::Path::getName(p.value()));
		dbe.payLoad(p);
		return *this;
	}

	template<typename P>
	bool RepositoryCore<P>::createRepoDir()
	{
		if (!path_.empty())
		{
			return FileSystem::Directory::create(path_);
		}
	}

	/*
	PayLoad pl;
	CheckIn ci
	ci.open();
	CI::addFile()
	CI::addPackage()

	repository.check_in(ci)
	*/

}