#include "PhysicsService.h"
#include "PhysicsInit.h"

#include <Jolt/Jolt.h>
#include <Jolt/Core/JobSystem.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>

#include <thread>

JPH_SUPPRESS_WARNINGS

namespace Twisted
{
    PhysicsService::PhysicsService(Application* app, int priority)
        : Service(app, priority) {}

    PhysicsService::~PhysicsService() = default;

    void PhysicsService::OnInit()
    {
        TE::Physics::Initialize();

        m_jobSystem = std::make_unique<JPH::JobSystemThreadPool>(
            JPH::cMaxPhysicsJobs,
            JPH::cMaxPhysicsBarriers,
            static_cast<int>(std::thread::hardware_concurrency()) - 1
        );
    }

    void PhysicsService::OnTerminate()
    {
        m_jobSystem.reset();
        TE::Physics::Shutdown();
    }

    JPH::JobSystem& PhysicsService::GetJobSystem()
    {
        return *m_jobSystem;
    }
}
