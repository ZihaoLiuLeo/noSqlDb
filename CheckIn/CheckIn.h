#pragma once
/////////////////////////////////////////////////////////////////////
// CheckIn.h - CheckIn mathods for repository                      //
// ver 1.0                                                         //
// Zihao Liu, Spring 2018                                          //
/////////////////////////////////////////////////////////////////////


#include <string>
#include "../RepositoryCore/RepositoryCore.h"

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

	CheckIn<P>& addFile(FileName fn); // open checkin then store info in db, store file in repo, add version to filename, close then update version
	CheckIn<P>& addPackage(PackageName pn);
	
	CheckIn<P>& setStatus(Status isOpen) { isOpen = isOpen; }

	bool ableToSetStatus(); // fidn all dependent file for this check in
private:
	RepositoryCore<P>& rc_;
	Status isOpen = true;
};