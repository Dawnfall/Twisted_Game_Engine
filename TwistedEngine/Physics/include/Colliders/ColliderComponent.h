#pragma once

#include "AppCore.h"
#include "AComponent.h"
#include "WorldRegistry.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <yaml-cpp/yaml.h>

namespace Twisted
{
    enum class ColliderShape { Box, Sphere, Capsule };
    enum class MotionType    { Static, Dynamic, Kinematic };
}

namespace YAML
{
    template<> struct convert<Twisted::ColliderShape>
    {
        static Node encode(const Twisted::ColliderShape& v) { return Node(static_cast<int>(v)); }
        static bool decode(const Node& n, Twisted::ColliderShape& v)
        {
            if (!n.IsScalar()) return false;
            v = static_cast<Twisted::ColliderShape>(n.as<int>());
            return true;
        }
    };
    template<> struct convert<Twisted::MotionType>
    {
        static Node encode(const Twisted::MotionType& v) { return Node(static_cast<int>(v)); }
        static bool decode(const Node& n, Twisted::MotionType& v)
        {
            if (!n.IsScalar()) return false;
            v = static_cast<Twisted::MotionType>(n.as<int>());
            return true;
        }
    };
}

namespace Twisted
{

    class TWISTED_API ColliderComponent : public AComponent
    {
        TCLASS_BODY()
    public:

        ColliderComponent(Entity e) : AComponent(e) {}

        TPROPERTY()
        ColliderShape shape = ColliderShape::Box;

        TPROPERTY()
        Vec3f halfExtents = { 0.5f, 0.5f, 0.5f };

        TPROPERTY()
        float radius = 0.5f;

        TPROPERTY()
        float capsuleHalfHeight = 0.5f;

        TPROPERTY()
        MotionType motionType = MotionType::Static;

        JPH::BodyID bodyID;
    };

    template<>
    void OnCreateComponent<ColliderComponent>(ColliderComponent& comp);
    template<>
    void OnDestroyComponent<ColliderComponent>(ColliderComponent& comp);
}
