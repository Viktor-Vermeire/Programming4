#include "Idle.h"
#include "RenderComponent.h"
#include "Minigin.h"
#include "GameObject.h"

void dae::Idle::Enter(GameObject*)
{
	
}

void dae::Idle::Execute(GameObject* gameObject)
{
	std::uniform_int_distribution<int> dist(0, 3);
	auto comp = gameObject->GetComponent<RenderComponent>();
	if (comp) {
		switch (dist(Minigin::RANDOM)) {
		case 0:
			comp->SetDirection(RenderComponent::RIGHT);
			return;
		case 1 :
			comp->SetDirection(RenderComponent::LEFT);
			return;
		case 2:
			comp->SetDirection(RenderComponent::UP);
			return;
		case 3:
			comp->SetDirection(RenderComponent::DOWN);
			return;
		}
		;
	}
}

void dae::Idle::Exit(GameObject*)
{
}
