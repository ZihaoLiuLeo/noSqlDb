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
#include "../Query/Query.h"


namespace RepoCore
{
	using namespace NoSqlDb;
	using namespace FileSystem;
	
	using FilePath = std::string;
	using Key = std::string;
	using Keys = std::vector<Key>;

	template<typename P>
	class RepositoryCore
	{
	public:
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

		Keys findFileWithVersion(FilePath path, int version);

		FilePath getOriginalName(const FilePath& path);

	private:
		DbCore<P> db_;
		FilePath path_ = "";
		DbElement<P>* p_dbe_path = nullptr;
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
		if (!path_.emtpy())
		{
			Key& key = p.value();
			DbElement<P>& dbe = db_[key];
			dbe.children(p.dependFiles());
			dbe.name(FileSystem::Path::getName(p.value()));
			dbe.filename(p.name());
			dbe.payLoad(p);

			db_[path_].children().push_back(key);
		}
		else
		{
			std::cout << "Failed to store files in database because the repository path has not been initialized. " << std::endl;
		}
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
	 
	template<typename P>
	Keys RepositoryCore<P>::findFileWithVersion(FilePath path, int version)
	{
		Query<P> q(db_);

		auto fileWithVersion = [=version](DbElement<P>& elem) {
			return ((elem.payLoad()).version() == version);
		};

		return q.from(db_[path].children()).fileWithVerison(version);
	}

	template<typename P>
	FilePath RepositoryCore<P>::getOriginalName(const FilePath& path)
	{
		return db_[path].filename();
	}

}