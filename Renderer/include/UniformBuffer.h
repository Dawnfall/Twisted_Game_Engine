#pragma once

#include "Application/TObject.h"
namespace Twisted
{
	class UniformBuffer: public TObject
	{
	public:
		UniformBuffer(const std::string& name);
		void Bind();
	private:

	};
}