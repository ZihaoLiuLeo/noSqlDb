#pragma once
/////////////////////////////////////////////////////////////////////
// RepositoryCore.h - Implements RepositoryCore for file managing  //
// ver 1.0                                                         //
// Zihao Liu, Fall 2019                                          //
/////////////////////////////////////////////////////////////////////

#include "../DbCore/DbCore.h"
#include "../Query/Query.h"
#include "../PayLoad/Payload.h"
#include <string>
#include "../FileSystem/FileSystemDemo/FileSystem.h"
#include "../Query/Query.h"
#include <stack>
#include <vector>


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

		RepositoryCore<P>& storePackage(P p);

		FilePath getPackDir(const Key& key);
		std::string getPackDescrip(const Key& key);

		bool containsPack(const Key& key);

		std::vector<Key*> browseAll(Key *pack);

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
	RepositoryCore<P>& RepositoryCore<P>::storePackage(P p)
	{
		if (!path_.emtpy())
		{
			Key& key = p.value();
			DbElement<P>& dbe = db_[key];
			dbe.children(p.dependPacks());
			dbe.name(path_);
			dbe.payLoad(p);

			//db_[path_].children().push_back(key);
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
	FilePath RepositoryCore<P>::getPackDir(const Key& key)
	{
		return db_[key].name();
	}

	template<typename P>
	std::string RepositoryCore<P>::getPackDescrip(const Key& key)
	{
		return db_[key].description();
	}


	template<typename P>
	bool RepositoryCore<P>::containsPack(const Key& key)
	{
		return db_.contains(key);
	}

	template<typename P>
	std::vector<Key*> RepositoryCore<P>::browseAll(Key *pack)
	{
		std::vector<Key*> packDirs;

		std::stack<Key*> keyStack;

		keyStack.push(pack);
		while (keyStack.size() > 0)
		{
			Key* pCNode = keyStack.top();
			keyStack.pop();
			packDirs.push_back(pCNode);
			size_t numChildren = db_[&pCNode].children().size();
			for (size_t i = 0; i < numChildren; ++i)
			{
				Key* k = db_[&pCNode].children()[numChildren - i - 1];

				if (std::find(packDirs.begin(), packDirs.end(), k) == packDirs.end())
				{
					keyStack.push(k);
				}
			}
		}
		
	}
}