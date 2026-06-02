#include "PhysicsInit.h"

#include <Jolt/Jolt.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include <cassert>
#include <iostream>
#include <thread>

JPH_SUPPRESS_WARNINGS

using namespace JPH;
using namespace JPH::literals;

namespace Layers {
    static constexpr ObjectLayer NON_MOVING = 0;
    static constexpr ObjectLayer MOVING     = 1;
    static constexpr ObjectLayer COUNT      = 2;
}

namespace BPLayers {
    static constexpr BroadPhaseLayer NON_MOVING{0};
    static constexpr BroadPhaseLayer MOVING{1};
    static constexpr uint32_t COUNT = 2;
}

class BPLayerInterfaceImpl final : public BroadPhaseLayerInterface
{
public:
    BPLayerInterfaceImpl() {
        mMap[Layers::NON_MOVING] = BPLayers::NON_MOVING;
        mMap[Layers::MOVING]     = BPLayers::MOVING;
    }
    uint GetNumBroadPhaseLayers() const override { return BPLayers::COUNT; }
    BroadPhaseLayer GetBroadPhaseLayer(ObjectLayer l) const override {
        JPH_ASSERT(l < Layers::COUNT);
        return mMap[l];
    }
private:
    BroadPhaseLayer mMap[Layers::COUNT];
};

class ObjVsBPFilter final : public ObjectVsBroadPhaseLayerFilter
{
public:
    bool ShouldCollide(ObjectLayer l1, BroadPhaseLayer l2) const override {
        if (l1 == Layers::NON_MOVING) return l2 == BPLayers::MOVING;
        return true;
    }
};

class ObjLayerFilter final : public ObjectLayerPairFilter
{
public:
    bool ShouldCollide(ObjectLayer l1, ObjectLayer l2) const override {
        if (l1 == Layers::NON_MOVING) return l2 == Layers::MOVING;
        return true;
    }
};

int main()
{
    TE::Physics::Initialize();

    TempAllocatorImpl  tempAlloc{10 * 1024 * 1024};
    JobSystemThreadPool jobSystem{cMaxPhysicsJobs, cMaxPhysicsBarriers,
                                  static_cast<int>(std::thread::hardware_concurrency()) - 1};

    BPLayerInterfaceImpl bpInterface;
    ObjVsBPFilter        objVsBP;
    ObjLayerFilter       objFilter;

    PhysicsSystem physics;
    physics.Init(1024, 0, 65536, 10240, bpInterface, objVsBP, objFilter);

    BodyInterface& bodies = physics.GetBodyInterface();

    // Static floor at y=0
    BodyCreationSettings floorSettings{
        new BoxShape(Vec3(100.f, 1.f, 100.f)),
        RVec3(0._r, -1._r, 0._r),
        Quat::sIdentity(),
        EMotionType::Static,
        Layers::NON_MOVING
    };
    bodies.CreateAndAddBody(floorSettings, EActivation::DontActivate);

    // Dynamic sphere dropped from y=10
    BodyCreationSettings sphereSettings{
        new SphereShape(0.5f),
        RVec3(0._r, 10._r, 0._r),
        Quat::sIdentity(),
        EMotionType::Dynamic,
        Layers::MOVING
    };
    BodyID sphere = bodies.CreateAndAddBody(sphereSettings, EActivation::Activate);

    physics.OptimizeBroadPhase();

    constexpr float dt    = 1.0f / 60.0f;
    constexpr int   steps = 120; // 2 seconds

    std::cout << "[PhysicsTest] Dropping sphere from y=10...\n";
    for (int i = 0; i < steps; ++i)
    {
        physics.Update(dt, 1, &tempAlloc, &jobSystem);
        if (i % 20 == 0)
        {
            RVec3 p = bodies.GetCenterOfMassPosition(sphere);
            std::cout << "  t=" << (i * dt) << "s  y=" << p.GetY() << "\n";
        }
    }

    RVec3 finalPos = bodies.GetCenterOfMassPosition(sphere);
    std::cout << "[PhysicsTest] Final y=" << finalPos.GetY()
              << "  (expected ~0.5, resting on floor)\n";

    assert(finalPos.GetY() > 0.3f && finalPos.GetY() < 1.0f);
    std::cout << "[PhysicsTest] PASSED\n";

    TE::Physics::Shutdown();
    return 0;
}
