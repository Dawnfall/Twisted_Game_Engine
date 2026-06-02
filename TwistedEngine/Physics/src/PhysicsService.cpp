#include "PhysicsService.h"

#include <Jolt/Jolt.h>
#include <Jolt/Core/JobSystemThreadPool.h>

#include <thread>
#include <Jolt/Physics/PhysicsSettings.h>

JPH_SUPPRESS_WARNINGS

namespace TE::Physics
{
    PhysicsService::PhysicsService(Twisted::Application* app, int priority)
        : Service(app, priority) {}

    PhysicsService::~PhysicsService() = default;

    void PhysicsService::OnInit()
    {
        m_jobSystem = std::make_unique<JPH::JobSystemThreadPool>(
            JPH::cMaxPhysicsJobs,
            JPH::cMaxPhysicsBarriers,
            static_cast<int>(std::thread::hardware_concurrency()) - 1
        );
    }

    void PhysicsService::OnFrame() {}

    void PhysicsService::OnTerminate()
    {
        m_jobSystem.reset();
    }
}
