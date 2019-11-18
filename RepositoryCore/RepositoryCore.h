#pragma once
/////////////////////////////////////////////////////////////////////
// RepositoryCore.h - Implements RepositoryCore for file managing  //
// ver 1.0                                                         //
// Zihao Liu, Spring 2018                                          //
/////////////////////////////////////////////////////////////////////

#include "../DbCore/DbCore.h"
#include "../Query/Query.h"
#include "../PayLoad/Payload.h"

namespace RepoCore
{
	using namespace NoSqlDb;

	template<P>
	class RepositoryCore
	{
	public:
	private:
		DbCore<P>* p_Db;
	};

	/*
	PayLoad pl;
	CheckIn ci
	ci.open();
	CI::addFile()
	CI::addPackage()

	repository.check_in(ci)
	*/

}