#pragma once

#include "ColliderComponent.h"
#include "WorldRegistry.h"

namespace Twisted
{
    class TWISTED_API BoxColliderComponent : public ColliderComponent
    {
        TCLASS_BODY()
    public:
        BoxColliderComponent(Entity e) : ColliderComponent(e) {}
    };
}
