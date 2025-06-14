#include "FloatingComponent.h"
#include "ResourceManager.h"
#include "BaseComponent.h"
#include "GameObject.h"
#include "Minigin.h"
dae::FloatingComponent::FloatingComponent(GameObject& gameObject, float movementSpeed): BaseComponent(gameObject), 
m_MovementSpeed{movementSpeed}
{
}

void dae::FloatingComponent::MoveTarget()
{
	auto direction = glm::normalize(m_Target->GetWorldTransform().GetPosition() - GetOwner()->GetWorldTransform().GetPosition());
	auto localPos = GetOwner()->GetLocalTransform().GetPosition() + (direction * m_MovementSpeed * Minigin::DELTATIME);
	GetOwner()->SetPosition(localPos.x, localPos.y);
	m_TimeFloating += Minigin::DELTATIME;
}

void dae::FloatingComponent::MoveToGrid()
{
	auto direction = glm::normalize(m_GridTarget - GetOwner()->GetWorldTransform().GetPosition());
	auto distance = m_GridTarget - GetOwner()->GetWorldTransform().GetPosition();
	//auto res = glm::length(distance);
	glm::vec3 localPos;
	if (glm::length(distance) < glm::length(direction * m_MovementSpeed * Minigin::DELTATIME)) {
		localPos = GetOwner()->GetLocalTransform().GetPosition() + distance;
		m_InGrid = true;
	}
	else {
		localPos = GetOwner()->GetLocalTransform().GetPosition() + (direction * m_MovementSpeed * Minigin::DELTATIME);
		
	}
	GetOwner()->SetPosition(localPos.x, localPos.y);
}

void dae::FloatingComponent::SetGridTarget(const glm::vec3& gridTarget)
{
	m_GridTarget = gridTarget;
}

void dae::FloatingComponent::SetPlayerTarget(GameObject* target)
{
	m_Target = target;
	m_TimeFloating = 0;
}

void dae::FloatingComponent::SetInGrid(bool value)
{
	m_InGrid = value;
}

bool dae::FloatingComponent::InGrid()
{
	return m_InGrid;
}

float dae::FloatingComponent::GetTimeFloating()
{
	return m_TimeFloating;
}
