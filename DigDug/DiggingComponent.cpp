#include "DiggingComponent.h"
#include "GameObject.h"

dae::DiggingComponent::DiggingComponent(GameObject& gameObject): BaseComponent(gameObject)
{
}

void dae::DiggingComponent::Dig()
{
	//if (GetOwner()->GetWorldTransform().GetPosition())
}
