#pragma once
/////////////////////////////////////////////////////////////////////
// Query.h - Implements Query to the noSqlDb            //
// ver 1.0                                                         //
// Zihao Liu, Spring 2018       //
/////////////////////////////////////////////////////////////////////
#include "../DbCore/DbCore.h"
#include <regex>

namespace NoSqlDb
{
	template<typename T>
	class Conditions
	{
	public:
		using RegExp = const std::string&;
		Conditions() {}

		Conditions& dbElement(DbElement<T>& dbe) { p_dbe = &dbe; return *this; }

		bool match();
		bool matchName();
		bool matchDescription();
		bool matchDateBefore();
		bool matchDateAfter();

		std::string& name() { return nameReg; }
		std::string name() const { return nameReg; }
		void name(RegExp regExp) { nameReg = regExp; }

		std::string& description() { return desReg; }
		std::string description() const { return desReg; }
		void description(RegExp regExp) { desReg = regExp; }

		DateTime& beforeDate() { return *bDate; }
		DateTime beforeDate() const { return *bDate; }
		void beforeDate(DateTime& dt) { bDate = &dt; }

		DateTime& afterDate() { return *aDate; }
		DateTime afterDate() const { return *aDate; }
		void afterDate(DateTime& dt) { aDate = &dt; }

	private:
		DbElement<T>* p_dbe = nullptr;

		std::string nameReg = "";
		std::string desReg = "";

		DateTime* bDate = nullptr;
		DateTime* aDate = nullptr;
	};

	template<typename T>
	bool Conditions<T>::match()
	{
		return matchName() && matchDescription() && matchDateBefore() && matchDateAfter();
	}

	template<typename T>
	bool Conditions<T>::matchName()
	{
		if (nameReg == "")
			return true;
		std::regex re(nameReg);
		return std::regex_search(p_dbe->name(), re);
	}

	template<typename T>
	bool Conditions<T>::matchDescription()
	{
		if (desReg == "")
			return true;
		std::regex re(desReg);
		return std::regex_search(p_dbe->descrip(), re);
	}

	template<typename T>
	bool Conditions<T>::matchDateBefore()
	{
		if (bDate == nullptr)
			return true;
		return p_dbe->dateTime() < *bDate;
	}

	template<typename T>
	bool Conditions<T>::matchDateAfter()
	{
		if (aDate == nullptr)
			return true;
		return p_dbe->dateTime() > *aDate;
	}

	template<typename T>
	class Query
	{
	public:
		using Key = std::string;
		using Keys = std::vector<Key>;

		Query() {}
		Query(DbCore<T>& db) :db_(&db), keys_(db_->keys()) {}
		Query(const Query<T>& q);
		Query(Query<T>&& q);

		Query<T>& operator=(const Query<T>& q);
		Query<T>& operator=(Query<T> && q);

		Keys keys() { return keys_; }

		void from(Keys& keys) { keys_ = keys; }
		Query<T>& show(std::ostream& out = std::cout);

		Query<T>& select(Conditions<T> conds);

		template<typename CallObj>
		Query<T>& select(CallObj callobj);

	private:
		DbCore<T>* db_;
		Keys keys_;
	};

	//----< copy constructor >--------------------------------------------
	template<typename T>
	Query<T>::Query(const Query<T>& q)
	{
		keys_ = q.keys_;
		db_ = q.db_;
	}

	//----< move constructor >--------------------------------------------
	template<typename T>
	Query<T>::Query(Query<T>&& q)
	{
		keys_ = q.keys_;
		db_ = q.db_;
	}

	//----< copy assignment >--------------------------------------------
	template<typename T>
	Query<T>& Query<T>::operator=(const Query<T>& q)
	{
		if (this == &q) return *this;
		keys_ = q.keys_;
		db_ = q.db_;
		return *this;
	}

	//----< move assignment >--------------------------------------------
	template<typename T>
	Query<T>& Query<T>::operator=(Query<T>&& q)
	{
		if (this == &q) return *this;
		keys_ = q.keys_;
		db_ = q.db_;
		return *this;
	}

	template<typename T>
	Query<T>& Query<T>::select(Conditions<T> conds)
	{
		Keys new_keys;
		for (auto k : keys_)
		{
			conds.dbElement((*db_)[k]);
			if (conds.match())
			{
				new_keys.push_back(k);
			}
		}
		keys_ = new_keys;
		return *this;
	}

	template<typename T>
	template<typename CallObj>
	Query<T>& Query<T>::select(CallObj callobj)
	{
		Keys new_keys;
		for (auto k : keys_)
		{
			if (callobj((*db_)[k]))
			{
				new_keys.push_back(k);
			}
		}
		keys_ = new_keys;
		return *this;
	}

	template<typename T>
	Query<T>& Query<T>::show(std::ostream& out)
	{
		out << "\n  ";
		for (auto k : keys_)
		{
			out << k << " ";
		}
		return *this;
	}
 }