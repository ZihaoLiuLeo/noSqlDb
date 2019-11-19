#pragma once
/////////////////////////////////////////////////////////////////////
// CheckIn.h - CheckIn mathods for repository                      //
// ver 1.0                                                         //
// Zihao Liu, Spring 2018                                          //
/////////////////////////////////////////////////////////////////////


#include <string>
#include "../PayLoad/Payload.h"
#include "../RepositoryCore/RepositoryCore.h"

namespace RepoCore {
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
		CheckIn<P>& addPackage(PackageName pn);

		CheckIn<P>& setStatus(Status status)

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
		if (isOpen)
		{
			rc_.store(p);
			//TODO denpendency check
			/*
			open p.value();
			new filename: filename + version.num(), copy content
			add to rc_.path();
			*/
		}
		return *this;
	}

	template<typename P>
	bool CheckIn<P>::ableToSetStatus()
	{
		/*
		for d in p.depent
			find d in rc_[p.value].children()
		*/
		return true
	}
}