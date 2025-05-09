#include "IsDoneRunning.h"
#include "GameObject.h"
#include "MovementComponent.h"

bool dae::IsDoneRunning::IsMet(GameObject* gameObject)
{
    auto movement = gameObject->GetComponent<MovementComponent>();
    if (movement) {
        return !movement->IsRunning();
    }
    return false;
}
