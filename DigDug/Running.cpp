#include "Running.h"
#include "GameObject.h"
#include "MovementComponent.h"
void dae::Running::Enter(GameObject* gameObject)
{
	auto movement = gameObject->GetComponent<MovementComponent>();
	if (movement != nullptr) {
		movement->ResetMovement();
	}
}
void dae::Running::Execute(GameObject* gameObject)
{
	auto movement = gameObject->GetComponent<MovementComponent>();
	if (movement != nullptr) {
		movement->ExecuteMove();
	}
}

void dae::Running::Exit(GameObject* gameObject)
{
	auto movement = gameObject->GetComponent<MovementComponent>();
	if (movement != nullptr) {
		movement->SetWantsToMove(false);
	}
}
