#pragma once

#include "Application/Service.h"

#include <memory>

namespace JPH { class JobSystemThreadPool; }

namespace TE::Physics
{
    class PhysicsService : public Twisted::Service
    {
    public:
        PhysicsService(Twisted::Application* app, int priority);
        ~PhysicsService() override;

        void OnInit()      override;
        void OnFrame()     override;
        void OnTerminate() override;

    private:
        std::unique_ptr<JPH::JobSystemThreadPool> m_jobSystem;
    };
}
