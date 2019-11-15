#pragma once
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../DbCore/DbCore.h"


namespace NoSqlDb 
{
	using namespace XmlProcessing;
	using Xml = std::string;
	using Sptr = std::shared_ptr<AbstractXmlElement>;

	template<typename T>
	class Persist
	{
	public:
		Persist(DbCore<T>& db) :p_db(&db) {};

		Xml toXml();
		DbCore<T> fromXml(const Xml& xml);
	private:
		DbCore<T>* p_db;
	};

	template<typename T>
	Xml Persist<T>::toXml()
	{
		std::cout << "\n  Creating XML representation of Mock Database using XmlDocument";
		std::cout << "\n ----------------------------------------------------------------";

		Sptr pDb = makeTaggedElement("db");
		pDb->addAttrib("type", "testDb");
		Sptr pDocElem = makeDocElement(pDb);
		XmlDocument xDoc(pDocElem);		

		for (auto key : p_db->keys())
		{
			Sptr pRecord = makeTaggedElement("dbrecord");
			pDb->addChild(pRecord);
			Sptr pKey = makeTaggedElement("key", key);
			pRecord->addChild(pKey);

			Sptr pValue = makeTaggedElement("value");
			pRecord->addChild(pValue);
			Sptr pName = makeTaggedElement("name", (*p_db)[key].name());
			pValue->addChild(pName);		
			Sptr pDes = makeTaggedElement("description", (*p_db)[key].descrip());
			pValue->addChild(pDes);
			Sptr pDatetime = makeTaggedElement("datetime", (*p_db)[key].dateTime());
			pValue->addChild(pDatetime);

			for (auto ckey : (*p_db)[key].children())
			{
				Sptr pChildren = makeTaggedElement("children", ckey);
				pValue->addChild(pChildren);
			}
			
		}

		std::string xml = xDoc.toString();
		std::cout << xml << std::endl;

		return xml;
	}

	template<typename T>
	DbCore<T> Persist<T>::fromXml(const Xml& xml)
	{
		std::cout << "\n  Creating XmlDocument from XML string";
		std::cout << "\n --------------------------------------";

		DbCore<T> newDb;
		XmlDocument newXDoc(xml, XmlDocument::str);
		std::cout << newXDoc.toString();
		std::cout << "\n";
		std::vector<Sptr> found = newXDoc.element("db").select();
		if (found.size() > 0)
		{
			AbstractXmlElement::Attributes attribs = found[0]->attributes();
			if (attribs.size() > 0)
				std::cout << "\n  database type = " << "\"" << attribs[0].second << "\"\n";
		}

		std::cout << "\n  Building MockDb from XmlDocument";
		std::cout << "\n ----------------------------------";

		using Key = std::string;
		std::vector<Sptr> records = newXDoc.descendents("dbrecord").select();
		for (auto pRecord : records)
		{
			Key key;
			DbElement<T> elem;
			std::vector<Sptr> pChildren = pRecord->children();
			for (auto pChild : pChildren)
			{
				if (pChild->tag() == "key")
				{
					key = pChild->children()[0]->value();
				}
				else
				{
					std::vector<Sptr> pValueChildren = pChild->children();
					for (auto pValueChild : pValueChildren)
					{
						if (pValueChild->tag() == "name")
						{
							elem.name(pValueChild->children()[0]->value());
						}
						if (pValueChild->tag() == "description")
						{
							elem.descrip(pValueChild->children()[0]->value());
						}
						if (pValueChild->tag() == "datetime")
						{
							elem.dateTime(pValueChild->children()[0]->value());
						}
						if (pValueChild->tag() == "children")
						{
							elem.children().push_back(pValueChild->children()[0]->value());
						}
					}
				}
			}
			newDb[key] = elem;
		}
		return newDb;
	}
}