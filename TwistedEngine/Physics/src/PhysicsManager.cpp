#include "PhysicsManager.h"

#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Core/JobSystem.h>

JPH_SUPPRESS_WARNINGS

namespace
{
    namespace Layers
    {
        static constexpr JPH::ObjectLayer NON_MOVING = 0;
        static constexpr JPH::ObjectLayer MOVING     = 1;
        static constexpr JPH::ObjectLayer COUNT      = 2;
    }

    namespace BPLayers
    {
        static constexpr JPH::BroadPhaseLayer NON_MOVING{ 0 };
        static constexpr JPH::BroadPhaseLayer MOVING{ 1 };
        static constexpr JPH::uint            COUNT = 2;
    }

    class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
    {
    public:
        BPLayerInterfaceImpl()
        {
            m_map[Layers::NON_MOVING] = BPLayers::NON_MOVING;
            m_map[Layers::MOVING]     = BPLayers::MOVING;
        }
        JPH::uint            GetNumBroadPhaseLayers() const override { return BPLayers::COUNT; }
        JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer l) const override
        {
            JPH_ASSERT(l < Layers::COUNT);
            return m_map[l];
        }
    private:
        JPH::BroadPhaseLayer m_map[Layers::COUNT];
    };

    class ObjVsBPFilter final : public JPH::ObjectVsBroadPhaseLayerFilter
    {
    public:
        bool ShouldCollide(JPH::ObjectLayer l1, JPH::BroadPhaseLayer l2) const override
        {
            if (l1 == Layers::NON_MOVING) return l2 == BPLayers::MOVING;
            return true;
        }
    };

    class ObjLayerFilter final : public JPH::ObjectLayerPairFilter
    {
    public:
        bool ShouldCollide(JPH::ObjectLayer l1, JPH::ObjectLayer l2) const override
        {
            if (l1 == Layers::NON_MOVING) return l2 == Layers::MOVING;
            return true;
        }
    };
}

namespace Twisted
{
    PhysicsManager::PhysicsManager(World* w)
        : ManagerBase(w)
        , m_tempAllocator(10 * 1024 * 1024)
    {
        m_bpInterface = std::make_unique<BPLayerInterfaceImpl>();
        m_objVsBP     = std::make_unique<ObjVsBPFilter>();
        m_objFilter   = std::make_unique<ObjLayerFilter>();

        m_physicsSystem.Init(
            65536, 0, 65536, 10240,
            *m_bpInterface, *m_objVsBP, *m_objFilter
        );
    }

    PhysicsManager::~PhysicsManager() = default;

    void PhysicsManager::Step(float dt, JPH::JobSystem& jobSystem)
    {
        m_physicsSystem.Update(dt, 1, &m_tempAllocator, &jobSystem);
    }
}
