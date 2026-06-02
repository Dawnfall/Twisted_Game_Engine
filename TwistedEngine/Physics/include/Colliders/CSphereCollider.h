#pragma once

#include "ColliderComponent.h"
#include "WorldRegistry.h"

namespace Twisted
{
    class TWISTED_API SphereColliderComponent : public ColliderComponent
    {
        TCLASS_BODY()
    public:
        SphereColliderComponent(Entity e) : ColliderComponent(e) {}
    };
}
