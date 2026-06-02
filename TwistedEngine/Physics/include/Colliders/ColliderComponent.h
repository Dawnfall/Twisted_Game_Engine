#pragma once

#include "AppCore.h"
#include "AComponent.h"

namespace Twisted
{
    class TWISTED_API ColliderComponent : public AComponent
    {
    public:
        ColliderComponent(Entity e) : AComponent(e) {}
    };
}
