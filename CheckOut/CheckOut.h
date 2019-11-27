#pragma once
#include "../FileSystem/FileSystemDemo/FileSystem.h"
#include "../RepositoryCore/RepositoryCore.h"
#include <string>
#include <vector>

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

	template<typename P>
	CheckOut<P>& CheckOut<P>::retrievePack(std::string path, int version)
	{
		// get all wanted files
		if (!destPath_.empty()) {
			std::vector<std::string> fromFileKeys;
			fromFileKeys = rc_.findFileWithVersion(path, version);
			// copy all files into destiny path
			for (auto fn : fromFileKeys)
			{
				std::string oriName = rc_.getOriginalName(fn);
				std::string tn = FileSystem::Path::fileSpec(destPath_, oriName);
				File from(fn);
				from.open(File::in, File::text);
				File to(tn);
				to.open(File::out, File::text);
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