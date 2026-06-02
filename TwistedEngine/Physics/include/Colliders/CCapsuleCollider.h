#pragma once

#include "ColliderComponent.h"
#include "WorldRegistry.h"

namespace Twisted
{
    class TWISTED_API CapsuleColliderComponent : public ColliderComponent
    {
        TCLASS_BODY()
    public:
        CapsuleColliderComponent(Entity e) : ColliderComponent(e) {}
    };
}
