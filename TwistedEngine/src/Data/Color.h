#pragma once
#include <yaml-cpp/yaml.h>

namespace Twisted
{
	struct Color
	{
		float r, g, b, a;

		bool operator== (const Color& other)const
		{
			return
				r == other.r &&
				g == other.g &&
				b == other.b &&
				a == other.a;
		}

		bool operator!= (const Color& other)const { return !((*this) == other); }
	};

	const Color black = { 0.0f,0.0f,0.0f,1.0f };
	const Color white = { 1.0f,1.0f,1.0f,1.0f };
	const Color blue = { 0.0f,0.0f,1.0f,1.0f };
	const Color red = { 1.0f,0.0f,0.0f,1.0f };
	const Color green = { 0.0f,1.0f,0.0f,1.0f };

}

//namespace YAML
//{
//	template<>
//	struct convert<std::vector<std::string>> 
//	{
//		YAML::Node encode(const Twisted::Color& color)
//		{
//			YAML::Node node;
//			node.push_back(color.r);
//			node.push_back(color.g);
//			node.push_back(color.b);
//			node.push_back(color.a);
//		}
//
//		void decode(const YAML::Node& node,Twisted::Color& color)
//		{
//			color.r = node[0].as<float>();
//			color.g = node[1].as<float>();
//			color.b = node[2].as<float>();
//			color.a = node[3].as<float>();
//		}
//	};
//}

