#include "BaseComponent.h"
#include "memory"
#include "GameObject.h"

dae::BaseComponent::BaseComponent(GameObject& gameObject):
	m_pOwner {&gameObject}, m_ToBeDeleted{ false }
{
}

dae::BaseComponent::~BaseComponent()
{
}

void dae::BaseComponent::Update()
{
	
}
void dae::BaseComponent::Render()
{
}

dae::GameObject* dae::BaseComponent::GetOwner()
{
	return m_pOwner;
}

bool dae::BaseComponent::ShouldBeDeleted()
{
	return m_ToBeDeleted;
}

void dae::BaseComponent::SetToBeDeleted(bool toBeDeleted)
{
	m_ToBeDeleted = toBeDeleted;
}
