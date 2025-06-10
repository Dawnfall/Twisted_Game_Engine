#include "MetaFile.h"

namespace Twisted::Editor
{
	bool MetaFile::Read(const std::string& metaFilePath)
	{
		//pugi::xml_document doc;

		//auto result = doc.load(metaFilePath.c_str());
		//if (!result)
		//{
		//	TWISTED_WARN("Meta file failed to read: " + metaFilePath + " ,description: " + result.description());
		//	return false;
		//}

		//pugi::xml_node versionNode = doc.child("Version");
		//if (versionNode) { Version = versionNode.child_value(); }

		//pugi::xml_node uuidNode = doc.child("Uuid");
		//if (uuidNode) { Uuid = uuidNode.child_value(); }

		//pugi::xml_node assetNameNode = doc.child("AssetName");
		//if (assetNameNode) { AssetName = assetNameNode.child_value(); }

		//pugi::xml_node typeNode = doc.child("Type");
		//if (typeNode) { Type = typeNode.child_value(); }

		//pugi::xml_node assetPathNode = doc.child("AssetPath");
		//if (assetPathNode) { AssetPath = assetPathNode.child_value(); }

		return true;
	}

	bool MetaFile::Write(const std::string& metaFilePath)
	{
		//pugi::xml_document doc;

		//pugi::xml_node versionNode = doc.append_child("Version");
		//versionNode.append_child(pugi::node_pcdata).set_value(Version);

		//pugi::xml_node uuidNode = doc.append_child("Uuid");
		//versionNode.append_child(pugi::node_pcdata).set_value(Uuid);

		//pugi::xml_node assetNameNode = doc.append_child("AssetName");
		//versionNode.append_child(pugi::node_pcdata).set_value(AssetName);

		//pugi::xml_node typeNode = doc.append_child("Type");
		//versionNode.append_child(pugi::node_pcdata).set_value(Type);

		//pugi::xml_node assetPathNode = doc.append_child("AssetPath");
		//versionNode.append_child(pugi::node_pcdata).set_value(AssetPath);

		//if (!doc.save_file(metaFilePath.c_str()))
		//{
		//	TWISTED_WARN("Failed to save: " + metaFilePath);
		//	return false;
		//}
		return true;
	}
}