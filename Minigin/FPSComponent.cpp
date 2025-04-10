#include "FPSComponent.h"
#include "Minigin.h"
#include "GameObject.h"
#include "TextComponent.h"

dae::FPSComponent::FPSComponent(GameObject& gameObject):
	BaseComponent(gameObject)
{
	m_FPS = 0;
}

void dae::FPSComponent::Update()
{
	CalculateFPS();
	if (m_ToWriteComponent != nullptr)
		m_ToWriteComponent->SetText(std::to_string(m_FPS));
}

float dae::FPSComponent::GetFPS()
{
	return m_FPS;
}

void dae::FPSComponent::SetToWriteComponent(TextComponent* textComponent)
{
	m_ToWriteComponent = textComponent;
}

void dae::FPSComponent::CalculateFPS()
{
		m_FPS = 1000.f / dae::Minigin::DELTATIME;
}
