#include "HealthComponent.h"
#include "BaseComponent.h"
#include "GameObject.h"
#include "Minigin.h"
#include "iostream"
#include "RenderComponent.h"

dae::HealthComponent::HealthComponent(GameObject& gameObject, int health, int lives, float damageCooldown) :
	BaseComponent(gameObject), m_Health{ health }, m_Lives{ lives }, m_DamageCooldown{damageCooldown}
{
}
void dae::HealthComponent::SetHealth(int health)
{
	m_Health = health;
	if (m_Health < 1)
		Die();
}

void dae::HealthComponent::Update()
{
	if (m_CooldownTimer > 0.f) {
		m_CooldownTimer -= Minigin::DELTATIME;
	}
}

int dae::HealthComponent::GetHealth()
{
	return m_Health;
}

int dae::HealthComponent::GetLives()
{
	return m_Lives;
}

void dae::HealthComponent::AddObserver(IObserver* observer)
{
	m_Observers.emplace_back(observer);
}

void dae::HealthComponent::RemoveObserver(IObserver* observer)
{
	for (int looper{ 0 }; looper < m_Observers.size(); ++looper) {
		if (m_Observers.at(looper) != observer)
			continue;
		m_Observers.at(looper) = nullptr;
		m_Observers.erase(m_Observers.begin() + looper);
	}
}

void dae::HealthComponent::Notify(Event event)
{
	for (auto observer : m_Observers) {
		observer->onNotify(*GetOwner(), event);
	}
}

void dae::HealthComponent::Die()
{
	if (m_CooldownTimer <= 0) {
		m_CooldownTimer = m_DamageCooldown;
		--m_Lives;
		std::cout << "hiyo Silver";
		m_Health = 1;
		if (m_Lives <= 0) {
			GetOwner()->m_Active = false;
		}
		Notify(DEATH);
	}
}