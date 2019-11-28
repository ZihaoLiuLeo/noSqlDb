#pragma once
/////////////////////////////////////////////////////////////////////
// Browse.h - Implements RepositoryCore for file managing  //
// ver 1.0                                                         //
// Zihao Liu, Fall 2019                                          //
/////////////////////////////////////////////////////////////////////
#include "../RepositoryCore/RepositoryCore.h"
#include <string>
#include <vector>
#include "../Process/Process/Process.h"

namespace RepoCore {
	template<typename P>
	class Browse
	{
	public:
		Browse() = default;
		Browse<P>(RepositoryCore<P>& rc) : rc_(rc) ();

		Browse<P>& browsePack(std::string *pack);

	private:
		RepositoryCore<P>& rc_;
	};

	void callProcess(std::string appPath)
	{
		Process p;
		p.application(appPath);

		std::string cmdLine = "/A ../Process/Process.h";
		p.commandLine(cmdLine);

		std::cout << "\n  starting process: \"" << appPath << "\"";
		std::cout << "\n  with this cmdlne: \"" << cmdLine << "\"";
		p.create();

		CBP callback = []() { std::cout << "\n  --- child process exited with this message ---"; };
		p.setCallBackProcessing(callback);
		p.registerCallback();

		std::cout << "\n  after OnExit";
		std::cout.flush();
		char ch;
		std::cin.read(&ch, 1);
	}

	template<typename P>
	Browse<P>& Browse<P>::browsePack(std::string *pack)
	{
		std::vector<std::string*> packs = rc_.browseAll(pack);
		for (auto i : packs)
		{
			std::string dir = rc_.getPackDir(&i);
			std::string descrip = rc_.getPackDescrip(&i);
			std::cout << "Pack: " << &i << "desciption: " << descrip << std::endl;
			callProcess(dir);
		}
		return *this;
	}
}