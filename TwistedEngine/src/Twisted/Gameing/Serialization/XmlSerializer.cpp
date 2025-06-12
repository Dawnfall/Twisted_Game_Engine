//#include "twistedpch.h"
//#include "XmlSerializer.h"
//#include "Utils/GlmUtils.h"
////pugixml
////#include <pugixml/pugixml.hpp>
//
//namespace Twisted
//{
//	XmlSerializer::XmlSerializer() : doc(std::make_unique<pugi::xml_document>()) {}
//	XmlSerializer::~XmlSerializer() = default;
//
//	void XmlSerializer::Write(const std::string& name, int data)
//	{
//		pugi::xml_node* node = GetNode(name, true);
//		if (node)
//			node->text().set(data);
//	}
//	void XmlSerializer::Write(const std::string& name, unsigned int data)
//	{
//		pugi::xml_node* node = GetNode(name, true);
//		if (node)
//			node->text().set(data);
//	}
//	void XmlSerializer::Write(const std::string& name, long long data)
//	{
//		pugi::xml_node* node = GetNode(name, true);
//		if (node)
//			node->text().set(data);
//	}
//	void XmlSerializer::Write(const std::string& name, float data)
//	{
//		pugi::xml_node* node = GetNode(name, true);
//		if (node)
//			node->text().set(data);
//	}
//	void XmlSerializer::Write(const std::string& name, double data)
//	{
//		pugi::xml_node* node = GetNode(name, true);
//		if (node)
//			node->text().set(data);
//	}
//	void XmlSerializer::Write(const std::string& name, const std::string& data)
//	{
//		pugi::xml_node* node = GetNode(name, true);
//		if (node)
//			node->text().set(data.c_str());
//	}
//	void XmlSerializer::Write(const std::string& name, const glm::ivec2& data)
//	{
//		Write(name, Utils::toString(data).c_str());
//	}
//
//	bool XmlSerializer::Read(const std::string& name, int& data) const
//	{
//		pugi::xml_node* node = GetNode(name, false);
//		if (node)
//		{
//			data = node->text().as_int();
//			return true;
//		}
//		return false;
//	}
//	bool XmlSerializer::Read(const std::string& name, unsigned int& data) const
//	{
//		pugi::xml_node* node = GetNode(name, false);
//		if (node)
//		{
//			data = node->text().as_uint();
//			return true;
//		}
//		return false;
//	}
//	bool XmlSerializer::Read(const std::string& name, float& data) const
//	{
//		pugi::xml_node* node = GetNode(name, false);
//		if (node)
//		{
//			data = node->text().as_float();
//			return true;
//		}
//		return false;
//	}
//	bool XmlSerializer::Read(const std::string& name, double& data) const
//	{
//		pugi::xml_node* node = GetNode(name, false);
//		if (node)
//		{
//			data = node->text().as_double();
//			return true;
//		}
//		return false;
//	}
//	bool XmlSerializer::Read(const std::string& name, long long& data) const
//	{
//		pugi::xml_node* node = GetNode(name, false);
//		if (node)
//		{
//			data = node->text().as_llong();
//			return true;
//		}
//		return false;
//	}
//	bool XmlSerializer::Read(const std::string& name, std::string& data) const
//	{
//		pugi::xml_node* node = GetNode(name, false);
//		if (node)
//		{
//			data = node->text().as_string();
//			return true;
//		}
//		return false;
//	}
//	bool XmlSerializer::Read(const std::string& name, glm::ivec2& data)const
//	{
//		pugi::xml_node* node = GetNode(name, false);
//		if (node)
//		{
//			data = Utils::fromStringVec2i(node->text().as_string());
//			return true;
//		}
//		return false;
//	}
//
//	void XmlSerializer::SaveToFile(const std::filesystem::path& fileName, const XmlSerializer& ser)
//	{
//		ser.doc->save_file(fileName.c_str());
//	}
//	pugi::xml_node* XmlSerializer::GetNode(const std::string& path, bool doCreateIfMissing)const
//	{
//		std::istringstream stream(path);
//		std::string segment;
//		pugi::xml_node node = *doc;
//
//		while (std::getline(stream, segment, '/'))
//		{
//			if (node.child(segment.c_str()))
//				node = node.child(segment.c_str());
//			else if (doCreateIfMissing)
//				node = node.append_child(segment.c_str());
//			else
//				return nullptr;
//		}
//		return &node;
//	}
//
//	XmlSerializer XmlSerializer::LoadFromFile(const std::filesystem::path& fileName)
//	{
//		XmlSerializer ser;
//		pugi::xml_document doc;
//		if (!doc.load_file(fileName.c_str()))
//		{
//			TWISTED_WARN("Failed to load xmlFile: " + fileName.string());
//			return ser;
//		}
//		*ser.doc = std::move(doc);
//		return ser;
//	}
//
//
//}