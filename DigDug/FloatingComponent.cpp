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
	auto direction = glm::normalize(m_GridTarget.GetPosition() - GetOwner()->GetWorldTransform().GetPosition());
	auto distance = m_GridTarget.GetPosition() - GetOwner()->GetWorldTransform().GetPosition();
	auto res = glm::length(distance);
	glm::vec3 localPos;
	if (glm::length(distance) < glm::length(direction * m_MovementSpeed)) {
		localPos = GetOwner()->GetLocalTransform().GetPosition() + distance;
	}
	else {
		localPos = GetOwner()->GetLocalTransform().GetPosition() + (direction * m_MovementSpeed);
		
	}
	GetOwner()->SetPosition(localPos.x, localPos.y);
}

void dae::FloatingComponent::SetGridTarget(Transform gridTarget)
{
	m_GridTarget = gridTarget;
}

void dae::FloatingComponent::SetPlayerTarget(GameObject* target)
{
	m_Target = target;
}
