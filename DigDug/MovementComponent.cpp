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

void dae::MovementComponent::ResetMovement()
{
	m_TimeInMovement = 0;
	m_IsMoving = true;
	m_WantsToMove = false;
}

bool dae::MovementComponent::WantsToMove()
{
	return m_WantsToMove;
}

void dae::MovementComponent::SetWantsToMove(bool value)
{
	m_WantsToMove = value;
}

bool dae::MovementComponent::IsRunning()
{
	return m_IsMoving;
}

void dae::MovementComponent::SetIsDigger(bool digger)
{
	m_IsDigger = digger;
}

void dae::MovementComponent::Update()
{
	/*if (!m_IsMoving)
		return;
	auto& localTransform = GetOwner()->GetLocalTransform().GetPosition();
	if (m_TimeInMovement + Minigin::DELTATIME > m_TimePerMove) {
		m_IsMoving = false; //This has to go
		GetOwner()->SetPosition(localTransform.x + (m_CurrentMovement.x * (m_TimePerMove - m_TimeInMovement)), localTransform.y + m_CurrentMovement.y * (m_TimePerMove - m_TimeInMovement));
		return;
	}
	m_TimeInMovement += Minigin::DELTATIME;
	std::cout << "x: " << localTransform.x << "\n" << "y: " << localTransform.y << "\n";
	GetOwner()->SetPosition(localTransform.x + (m_CurrentMovement.x * Minigin::DELTATIME), localTransform.y + (m_CurrentMovement.y * Minigin::DELTATIME));*/
}

void dae::MovementComponent::ExecuteMove()
{
	m_IsMoving = true;
	auto& localTransform = GetOwner()->GetLocalTransform().GetPosition();
	//std::cout << "Time: " << Minigin::DELTATIME << "\n";
	if (m_TimeInMovement + Minigin::DELTATIME > m_TimePerMove) {
		m_IsMoving = false;
		GetOwner()->SetPosition(localTransform.x + (m_CurrentMovement.x * (m_TimePerMove - m_TimeInMovement)), localTransform.y + m_CurrentMovement.y * (m_TimePerMove - m_TimeInMovement));
		return;
	}
	m_TimeInMovement += Minigin::DELTATIME;
	//std::cout << "x: " << localTransform.x << "\n" << "y: " << localTransform.y << "\n";
	GetOwner()->SetPosition(localTransform.x + (m_CurrentMovement.x * Minigin::DELTATIME), localTransform.y + (m_CurrentMovement.y * Minigin::DELTATIME));
}

void dae::MovementComponent::Move(dae::RenderComponent::Direction direction)
{
	/*if (m_IsMoving) //Should be deletable
		return;*/
	if (!GetOwner()->GetParent()) return;
	//std::cout << "Being hit \n";
	//m_WantsToMove = true;
	
	HallwaysComponent* hallways = ((GetOwner()->GetParent() != nullptr) ? GetOwner()->GetParent()->GetComponent<HallwaysComponent>() : nullptr);
	if (hallways == nullptr)
		return;
	auto pos = GetOwner()->GetWorldTransform().GetPosition();
	//hallways->Dig(GetOwner()->GetWorldTransform().GetPosition(), GetOwner()->GetWorldTransform().GetPosition() + glm::vec3{m_DistancePerMove / m_TimePerMove, 0, 0});
	switch (direction) {
	case RenderComponent::RIGHT:
		pos.x = pos.x + m_DistancePerMove;
		m_WantsToMove = hallways->IsValidHallway(pos);
		m_CurrentMovement = { m_DistancePerMove / m_TimePerMove, 0 };
		break;
	case RenderComponent::DOWN:
		pos.y = pos.y + m_DistancePerMove;
		m_WantsToMove = hallways->IsValidHallway(pos);
		m_CurrentMovement = { 0 , m_DistancePerMove / m_TimePerMove };
		break;
	case RenderComponent::UP:
		pos.y = pos.y - m_DistancePerMove;
		m_WantsToMove = hallways->IsValidHallway(pos);
		m_CurrentMovement = { 0 , -m_DistancePerMove / m_TimePerMove };
		break;
	case RenderComponent::LEFT:
		pos.x = pos.x - m_DistancePerMove;
		m_WantsToMove = hallways->IsValidHallway(pos);
		m_CurrentMovement = { -m_DistancePerMove / m_TimePerMove, 0 };
		break;
	}
	if (m_IsDigger && m_WantsToMove) 
		hallways->Dig(GetOwner()->GetWorldTransform().GetPosition(), GetOwner()->GetWorldTransform().GetPosition() + glm::vec3{ m_CurrentMovement * m_TimePerMove, 0 });
}
