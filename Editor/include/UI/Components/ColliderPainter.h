#pragma once

#include "UI/ComponentPainter.h"

namespace Twisted::Editor
{
    class ColliderPainter : public ComponentPainter
    {
    public:
        void Paint(void* obj) override;
    };
}
