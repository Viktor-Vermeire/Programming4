#include "RotatorComponent.h"
#include "GameObject.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Minigin.h"

dae::RotatorComponent::RotatorComponent(GameObject& gameObject, float turnRate):
	BaseComponent(gameObject)
{
	m_TurnRate = turnRate;
}

void dae::RotatorComponent::SetTurnRate(float turnRate)
{
	m_TurnRate = turnRate;
}

void dae::RotatorComponent::Update()
{
	Rotate();
}

void dae::RotatorComponent::Rotate()
{
	glm::vec4 point4D(GetOwner()->GetLocalTransform().GetPosition(), 0.0f);
	glm::vec4 rotatedPoint4D = glm::rotate(glm::mat4(1.0f), glm::radians(m_TurnRate * dae::Minigin::DELTATIME), glm::vec3(0.0f, 0.0f, 1.0f)) * point4D;
	GetOwner()->SetPosition(rotatedPoint4D.x, rotatedPoint4D.y);
}
