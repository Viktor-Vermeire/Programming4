#include "HealthComponent.h"
#include "BaseComponent.h"
#include "GameObject.h"

dae::HealthComponent::HealthComponent(GameObject& gameObject, int health, int lives) :
	BaseComponent(gameObject), m_Health{ health }, m_Lives{ lives }
{
}
void dae::HealthComponent::SetHealth(int health)
{
	m_Health = health;
	if (m_Health < 1)
		Die();
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
	--m_Lives;
	m_Health = 1;
	Notify(DEATH);
}