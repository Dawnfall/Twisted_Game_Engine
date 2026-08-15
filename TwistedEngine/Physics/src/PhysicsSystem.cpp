#include "PhysicsSystem.h"
#include "PhysicsService.h"
#include "Colliders/ColliderComponent.h"
#include "Components/CTransform.h"
#include "Application/Application.h"

#include <Jolt/Jolt.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystem.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>

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
    PhysicsSystem::PhysicsSystem(World* world)
        : TSystem(world)
        , m_service(Application::GetInstance().GetService<PhysicsService>())
    {
        if (!m_service)
        {
            Pause();
            return;
        }

        m_tempAllocator = std::make_unique<JPH::TempAllocatorImpl>(10 * 1024 * 1024);

        m_bpInterface = std::make_unique<BPLayerInterfaceImpl>();
        m_objVsBP     = std::make_unique<ObjVsBPFilter>();
        m_objFilter   = std::make_unique<ObjLayerFilter>();

        m_physicsSystem = std::make_unique<JPH::PhysicsSystem>();
        m_physicsSystem->Init(
            65536, 0, 65536, 10240,
            *m_bpInterface, *m_objVsBP, *m_objFilter
        );
    }

    PhysicsSystem::~PhysicsSystem()
    {
        m_bodies.clear();
        m_physicsSystem.reset();
        m_bpInterface.reset();
        m_objVsBP.reset();
        m_objFilter.reset();
        m_tempAllocator.reset();
    }

    void PhysicsSystem::Update(float deltaTime)
    {
        const float step = m_service->GetConfig().fixedStep;
        m_accumulator += deltaTime;
        while (m_accumulator >= step)
        {
            m_physicsSystem->Update(step, 1, m_tempAllocator.get(), &m_service->GetJobSystem());
            m_accumulator -= step;
        }
    }

    JPH::PhysicsSystem& PhysicsSystem::GetPhysicsSystem()
    {
        return *m_physicsSystem;
    }

    JPH::BodyID PhysicsSystem::RegisterCollider(ColliderComponent& comp)
    {
        if (!m_physicsSystem) return {};

        JPH::EMotionType joltMotion;
        JPH::ObjectLayer layer;
        switch (comp.motionType)
        {
            case MotionType::Static:
                joltMotion = JPH::EMotionType::Static;
                layer      = Layers::NON_MOVING;
                break;
            case MotionType::Kinematic:
                joltMotion = JPH::EMotionType::Kinematic;
                layer      = Layers::MOVING;
                break;
            default:
                joltMotion = JPH::EMotionType::Dynamic;
                layer      = Layers::MOVING;
                break;
        }

        JPH::Ref<JPH::Shape> shape;
        switch (comp.shape)
        {
            case ColliderShape::Sphere:
                shape = new JPH::SphereShape(comp.radius);
                break;
            case ColliderShape::Capsule:
                shape = new JPH::CapsuleShape(comp.capsuleHalfHeight, comp.radius);
                break;
            default:
                shape = new JPH::BoxShape(JPH::Vec3(comp.halfExtents.x, comp.halfExtents.y, comp.halfExtents.z));
                break;
        }

        TransformComponent* tf  = comp.GetWorld()->TryGetComponent<TransformComponent>(comp.GetID());
        Vec3f pos               = tf ? tf->GetWorldPosition() : Vec3f{ 0.0f, 0.0f, 0.0f };
        Quat  rot               = tf ? tf->GetWorldRotation() : Quat{ 1.0f, 0.0f, 0.0f, 0.0f };

        JPH::BodyCreationSettings settings(
            shape,
            JPH::RVec3(pos.x, pos.y, pos.z),
            JPH::Quat(rot.x, rot.y, rot.z, rot.w),
            joltMotion,
            layer
        );

        JPH::EActivation activation = (comp.motionType == MotionType::Static)
            ? JPH::EActivation::DontActivate
            : JPH::EActivation::Activate;

        auto& bi = m_physicsSystem->GetBodyInterface();
        return bi.CreateAndAddBody(settings, activation);
    }

    void PhysicsSystem::UnregisterCollider(ColliderComponent& comp)
    {
        if (comp.bodyID.IsInvalid()) return;
        if (!m_physicsSystem) return;
        auto& bi = m_physicsSystem->GetBodyInterface();
        bi.RemoveBody(comp.bodyID);
        bi.DestroyBody(comp.bodyID);
    }
}
