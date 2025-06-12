#pragma once
#include "AppCore.h"


namespace pugi
{
	class xml_document;
	class xml_node;
}

namespace Twisted
{
	class TWISTED_API XmlSerializer
	{
	public:
		XmlSerializer();
		~XmlSerializer();

		// Delete copy constructor and copy assignment operator
		XmlSerializer(const XmlSerializer&) = delete;
		XmlSerializer& operator=(const XmlSerializer&) = delete;

		XmlSerializer(XmlSerializer&&) noexcept = default;
		XmlSerializer& operator=(XmlSerializer&&) noexcept = default;

		void Write(const std::string& name, int data);
		void Write(const std::string& name, unsigned int data);
		void Write(const std::string& name, long long data);
		void Write(const std::string& name, float data);
		void Write(const std::string& name, double data);
		void Write(const std::string& name, const glm::ivec2& data);
		void Write(const std::string& name, const std::string& data);

		bool Read(const std::string& name, int& data) const;
		bool Read(const std::string& name, unsigned int& data) const;
		bool Read(const std::string& name, long long& data) const;
		bool Read(const std::string& name, float& data) const;
		bool Read(const std::string& name, double& data) const;
		bool Read(const std::string& name, glm::ivec2& data)const;
		bool Read(const std::string& name, std::string& data) const;

		static void SaveToFile(const std::filesystem::path& fileName, const XmlSerializer& ser);
		static XmlSerializer LoadFromFile(const std::filesystem::path& fileName);
	private:

		URef<pugi::xml_document> doc;
		pugi::xml_node* GetNode(const std::string& path, bool createIfNotExist)const;
	};


}