#pragma once
/////////////////////////////////////////////////////////////////////
// CheckOut.h - CheckOut mathods for repository                    //
// ver 1.0                                                         //
// Zihao Liu, fall 2019                                            //
/////////////////////////////////////////////////////////////////////
#include "../FileSystem/FileSystemDemo/FileSystem.h"
#include "../RepositoryCore/RepositoryCore.h"
#include <string>
#include <vector>
#include <regex>


namespace RepoCore {
	using namespace FileSystem;

	template<typename P>
	class CheckOut
	{
	public:
		Checkout() = default;
		CheckOut<P>(RepositoryCore<P>& rc) : rc_(rc) {};

		CheckOut<P>& retrievePack(std::string path, int version);

		std::string& destiny() { return destPath_; }
		std::string destiny() const { return destPath_; }
		void destiny(const std::string& path) { destPath_ = path; }

	private:
		RepositoryCore<P>& rc_;
		std::string destPath_;
	};

	std::vector<std::string> Split(const std::string& str, const std::string& regex)
	{
		std::regex r(regex);
		return { std::sregex_token_iterator(str.begin(), str.end(), r, -1), std::sregex_token_iterator() };
	}

	template<typename P>
	CheckOut<P>& CheckOut<P>::retrievePack(std::string packName, int version = 1)
	{
		// get all wanted files
		if (!destPath_.empty()) {
			std::vector<std::string> fromFileKeys;
			packDir = rc_.getPackDir(packName);
			fromFileKeys = FileSystem::Directory::getFiles(packDir);

			// copy all files into destiny path
			for (auto fn : fromFileKeys)
			{
				std::vector<std::string> tnv = Split(fn, "//.");
				if (tnv[tnv.size() - 1] == std::to_string(version))
				{
					std::string oriName = tnv[0] + tnv[1];
					std::string tn = FileSystem::Path::fileSpec(destPath_, oriName);
					File from(fn);
					from.open(File::in);
					File to(tn);
					to.open(File::out);
					while (to.isGood() && from.isGood())
					{
						std::string temp = from.getLine();
						to.putLine(temp);
						to.putLine("\n");
					}
					to.close();
					from.close();
					FileSystem::File::remove(fn);
				}
			}
		}
	}
}