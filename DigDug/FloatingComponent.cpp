#include "FloatingComponent.h"
#include "ResourceManager.h"
#include "BaseComponent.h"
#include "GameObject.h"
dae::FloatingComponent::FloatingComponent(GameObject& gameObject, float movementSpeed): BaseComponent(gameObject), 
m_MovementSpeed{movementSpeed}
{
}

void dae::FloatingComponent::MoveTarget()
{
	auto direction = glm::normalize(m_Target->GetWorldTransform().GetPosition() - GetOwner()->GetWorldTransform().GetPosition());
	auto localPos = GetOwner()->GetLocalTransform().GetPosition() + (direction * m_MovementSpeed);
	GetOwner()->SetPosition(localPos.x, localPos.y);
}

void dae::FloatingComponent::MoveToGrid()
{
	auto direction = glm::normalize(m_GridTarget - GetOwner()->GetWorldTransform().GetPosition());
	auto distance = m_GridTarget - GetOwner()->GetWorldTransform().GetPosition();
	//auto res = glm::length(distance);
	glm::vec3 localPos;
	if (glm::length(distance) < glm::length(direction * m_MovementSpeed)) {
		localPos = GetOwner()->GetLocalTransform().GetPosition() + distance;
		m_InGrid = true;
	}
	else {
		localPos = GetOwner()->GetLocalTransform().GetPosition() + (direction * m_MovementSpeed);
		
	}
	GetOwner()->SetPosition(localPos.x, localPos.y);
}

void dae::FloatingComponent::SetGridTarget(glm::vec3 gridTarget)
{
	m_GridTarget = gridTarget;
}

void dae::FloatingComponent::SetPlayerTarget(GameObject* target)
{
	m_Target = target;
}

void dae::FloatingComponent::SetInGrid(bool value)
{
	m_InGrid = value;
}

bool dae::FloatingComponent::InGrid()
{
	return m_InGrid;
}
