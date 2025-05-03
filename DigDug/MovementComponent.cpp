#include "MovementComponent.h"
#include "Minigin.h"
#include "GameObject.h"
#include "AnimationComponent.h"
#include "iostream"
#include "HallwaysComponent.h"
#include <glm.hpp>

dae::MovementComponent::MovementComponent(GameObject& gameObject) : BaseComponent(gameObject),
m_DistancePerMove{ 0 }, m_TimePerMove{ 0 }, m_TimeInMovement{ 0 }, m_IsMoving{false}
{
}

void dae::MovementComponent::SetDistancePerMove(int distance)
{
	m_DistancePerMove = distance;
}
void dae::MovementComponent::SetTimePerMove(float time) {
	m_TimePerMove = time;
}

void dae::MovementComponent::SetHallways(GameObject* hallways)
{
	m_Hallways = hallways;
}

void dae::MovementComponent::Update()
{
	if (!m_IsMoving)
		return;
	auto& localTransform = GetOwner()->GetLocalTransform().GetPosition();
	if (m_TimeInMovement + Minigin::DELTATIME > m_TimePerMove) {
		m_IsMoving = false;
		GetOwner()->SetPosition(localTransform.x + (m_CurrentMovement.x * (m_TimePerMove - m_TimeInMovement)), localTransform.y + m_CurrentMovement.y * (m_TimePerMove - m_TimeInMovement));
		return;
	}
	m_TimeInMovement += Minigin::DELTATIME;
	std::cout << "x: " << localTransform.x << "\n" << "y: " << localTransform.y << "\n";
	GetOwner()->SetPosition(localTransform.x + (m_CurrentMovement.x * Minigin::DELTATIME), localTransform.y + (m_CurrentMovement.y * Minigin::DELTATIME));
}

void dae::MovementComponent::Move(dae::AnimationComponent::Direction direction)
{
	if (m_IsMoving)
		return;
	m_IsMoving = true;
	m_TimeInMovement = 0;
	HallwaysComponent* hallways = ((m_Hallways != nullptr) ? m_Hallways->GetComponent<HallwaysComponent>() : nullptr);
	if (hallways == nullptr)
		return;
	//hallways->Dig(GetOwner()->GetWorldTransform().GetPosition(), GetOwner()->GetWorldTransform().GetPosition() + glm::vec3{m_DistancePerMove / m_TimePerMove, 0, 0});
	switch (direction) {
	case AnimationComponent::RIGHT:
		m_CurrentMovement = { m_DistancePerMove / m_TimePerMove, 0 };
		break;
	case AnimationComponent::DOWN:
		m_CurrentMovement = { 0 , m_DistancePerMove / m_TimePerMove };
		break;
	case AnimationComponent::UP:
		m_CurrentMovement = { 0 , -m_DistancePerMove / m_TimePerMove };
		break;
	case AnimationComponent::LEFT:
		m_CurrentMovement = { -m_DistancePerMove / m_TimePerMove, 0 };
		break;
	}
	//auto loc = GetOwner()->GetWorldTransform().GetPosition() + glm::vec3{ -1, 1, 0};
	hallways->Dig(GetOwner()->GetWorldTransform().GetPosition(), GetOwner()->GetWorldTransform().GetPosition() + glm::vec3{ m_CurrentMovement * m_TimePerMove, 0 });
}
