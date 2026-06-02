#pragma once
#include "UI/ComponentPainter.h"
#include "Reflection/PropertyInfo.h"
#include <vector>

namespace Twisted::Editor
{
	class GenericComponentPainter : public ComponentPainter
	{
	public:
		explicit GenericComponentPainter(const std::vector<Twisted::PropertyInfo>* props);
		void Paint(void* obj) override;
	private:
		const std::vector<Twisted::PropertyInfo>* m_props;
	};
}
