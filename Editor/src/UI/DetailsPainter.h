#pragma once
#include <string>

namespace Twisted::Editor
{
	class DetailsPainter
	{
	public:
		virtual void Paint(void* obj) = 0;
	};

}