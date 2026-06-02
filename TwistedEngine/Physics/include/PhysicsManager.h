#pragma once

#include "AppCore.h"
#include "ManagerBase.h"
#include "Entity.h"
#include "WorldRegistry.h"

#include <Jolt/Jolt.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyID.h>

#include <memory>
#include <unordered_map>

namespace JPH
{
    class JobSystem;
    class BroadPhaseLayerInterface;
    class ObjectVsBroadPhaseLayerFilter;
    class ObjectLayerPairFilter;
}

namespace Twisted
{
    class TWISTED_API PhysicsManager : public ManagerBase
    {
        TCLASS_BODY()
    public:
        PhysicsManager(World* w);
        ~PhysicsManager();

        void Step(float dt, JPH::JobSystem& jobSystem);

    private:
        std::unique_ptr<JPH::BroadPhaseLayerInterface>      m_bpInterface;
        std::unique_ptr<JPH::ObjectVsBroadPhaseLayerFilter> m_objVsBP;
        std::unique_ptr<JPH::ObjectLayerPairFilter>         m_objFilter;

        JPH::TempAllocatorImpl                    m_tempAllocator;
        JPH::PhysicsSystem                        m_physicsSystem;

        std::unordered_map<EntityID, JPH::BodyID> m_bodies;
    };
}
