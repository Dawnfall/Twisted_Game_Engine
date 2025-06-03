#pragma once
#include <string>

namespace Twisted::Editor
{
	template<typename T>
	class DetailsRenderer
	{
	public:
		void RenderDetails(T* component) = delete;
		std::string GetName() = delete;
	};
}