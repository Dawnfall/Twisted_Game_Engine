#pragma once

#include "AppCore.h"
#include "SystemBase.h"
#include "Entity.h"
#include "WorldRegistry.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>

#include <memory>
#include <unordered_map>

namespace JPH
{
    class BroadPhaseLayerInterface;
    class ObjectVsBroadPhaseLayerFilter;
    class ObjectLayerPairFilter;
    class TempAllocator;
    class PhysicsSystem;
}

namespace Twisted
{
    class PhysicsService;
    class ColliderComponent;

    class TWISTED_API PhysicsSystem : public TSystem<PhysicsSystem>
    {
        TCLASS_BODY()
    public:
        PhysicsSystem(World* world);
        ~PhysicsSystem() override;

        void Update(float deltaTime) override;

        JPH::BodyID RegisterCollider(ColliderComponent& comp);
        void        UnregisterCollider(ColliderComponent& comp);

        JPH::PhysicsSystem& GetPhysicsSystem();

    private:
        PhysicsService* m_service = nullptr;

        std::unique_ptr<JPH::BroadPhaseLayerInterface>      m_bpInterface;
        std::unique_ptr<JPH::ObjectVsBroadPhaseLayerFilter> m_objVsBP;
        std::unique_ptr<JPH::ObjectLayerPairFilter>         m_objFilter;
        std::unique_ptr<JPH::TempAllocator>                 m_tempAllocator;
        std::unique_ptr<JPH::PhysicsSystem>                 m_physicsSystem;

        std::unordered_map<EntityID, JPH::BodyID>           m_bodies;

        float m_accumulator = 0.0f;
    };
}
