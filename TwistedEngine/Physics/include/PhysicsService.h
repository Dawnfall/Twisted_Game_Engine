#pragma once

#include "AppCore.h"
#include "Application/Service.h"

#include <Jolt/Jolt.h>

#include <memory>

namespace JPH { class JobSystem; }

namespace Twisted
{
    struct TWISTED_API PhysicsConfig
    {
        float fixedStep = 1.0f / 60.0f;
    };

    class TWISTED_API PhysicsService : public Service
    {
    public:
        PhysicsService(Application* app, int priority);
        ~PhysicsService() override;

        void OnInit()      override;
        void OnTerminate() override;

        JPH::JobSystem&      GetJobSystem();
        PhysicsConfig&       GetConfig()       { return m_config; }
        const PhysicsConfig& GetConfig() const { return m_config; }

    private:
        PhysicsConfig                   m_config;
        std::unique_ptr<JPH::JobSystem> m_jobSystem;
    };
}
