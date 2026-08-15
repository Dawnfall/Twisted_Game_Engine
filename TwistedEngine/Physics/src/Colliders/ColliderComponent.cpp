#include "Colliders/ColliderComponent.h"
#include "PhysicsSystem.h"

namespace Twisted
{
    template<>
    void OnCreateComponent<ColliderComponent>(ColliderComponent& comp)
    {
        PhysicsSystem* ps = comp.GetWorld()->GetSystem<PhysicsSystem>();
        if (!ps) return;
        comp.bodyID = ps->RegisterCollider(comp);
    }

    template<>
    void OnDestroyComponent<ColliderComponent>(ColliderComponent& comp)
    {
        PhysicsSystem* ps = comp.GetWorld()->GetSystem<PhysicsSystem>();
        if (!ps) return;
        ps->UnregisterCollider(comp);
    }
}
