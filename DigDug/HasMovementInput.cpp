#include "HasMovementInput.h"
#include "GameObject.h"
#include "MovementComponent.h"

bool dae::HasMovementInput::IsMet(GameObject* gameObject)
{
    auto movement = gameObject->GetComponent<MovementComponent>();
    if (movement) {
        return movement->WantsToMove();
    }
    return false;
}
