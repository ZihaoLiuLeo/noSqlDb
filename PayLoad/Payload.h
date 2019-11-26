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

		std::vector<std::string>& categories() { return categories_; }
		std::vector<std::string> categories() const { return categories_; }
		void categories(const std::vector<std::string>& categories) { categories_ = categories; }

		std::vector<FileName>& dependFiles() { return dependFiles_; }
		std::vector<FileName> dependsFiles() const { return dependFiles_; }
		void dependFiles(const std::vector<FileName>& depends) { dependFiles_ = depends; }

		bool hasCategory(const std::string& category);


	private:
		std::string value_;
		std::vector<std::string> categories_;
		std::vector<FileName> dependFiles_;
	};

	inline bool PayLoad::hasCategory(const std::string& category)
	{
		return std::find(categories_.begin(), categories_.end(), category) != categories_.end();
	}
}

#endif
