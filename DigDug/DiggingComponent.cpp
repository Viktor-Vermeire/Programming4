#include "DiggingComponent.h"
#include "GameObject.h"
#include "HallwaysComponent.h"

dae::DiggingComponent::DiggingComponent(GameObject& gameObject, HallwaysComponent* component): BaseComponent(gameObject)
{
	m_pHallwaysComponent = component;
}

void dae::DiggingComponent::Dig()
{
}
