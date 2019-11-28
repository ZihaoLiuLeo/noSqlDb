#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <string>
#include <vector>
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"

namespace NoSqldb
{
	class PayLoad
	{
		using FileName = std::string;

	public:
		PayLoad() = default;
		PayLoad(const std::string& val) : value_(val) {}

		std::string& value() { return value_; }
		std::string value() const { return value_; }		
		void value(const std::string& value) { value_ = value; }

		std::string& name() { return name_; }
		std::string name() const { return name_; }
		void name(const std::string& name) { name_ = name; }

		std::vector<std::string>& categories() { return categories_; }
		std::vector<std::string> categories() const { return categories_; }
		void categories(const std::vector<std::string>& categories) { categories_ = categories; }

		std::vector<FileName>& dependPacks() { return dependPacks_; }
		std::vector<FileName> dependsFiles() const { return dependPacks_; }
		void dependPacks(const std::vector<FileName>& depends) { dependPacks_ = depends; }

		int& version() { return version_; }
		int version() const { return version_; }
		void version(const int& version) { version_ = version; }

		bool hasCategory(const std::string& category);


	private:
		std::string value_;
		std::string name_;
		std::vector<std::string> categories_;
		std::vector<FileName> dependPacks_;
		int version_ = 0;
	};

	inline bool PayLoad::hasCategory(const std::string& category)
	{
		return std::find(categories_.begin(), categories_.end(), category) != categories_.end();
	}
}

#endif
