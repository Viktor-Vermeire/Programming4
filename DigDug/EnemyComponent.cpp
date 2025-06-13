#include "EnemyComponent.h"
#include "GameObject.h"
#include "vector"
#include "Minigin.h"

dae::EnemyComponent::EnemyComponent(GameObject& gameObject, int maxInflation, int value, float timeTethered) : BaseComponent(gameObject), 
m_MaxInflation{ maxInflation }, m_Value{ value }, m_MaxTimeTethered{timeTethered} {
	m_Inflation = 0;
	m_CurrentTimeTethered = 0;
}

void dae::EnemyComponent::AddPlayers(GameObject* gameObject)
{
	m_Players.emplace_back(gameObject);
}

void dae::EnemyComponent::Update()
{
	if (m_Tethered) {
		m_CurrentTimeTethered += Minigin::DELTATIME;
		if (m_CurrentTimeTethered > m_MaxTimeTethered) {
			m_CurrentTimeTethered = 0;
			m_Tethered = false;
			m_Inflation = 0;
		}
	}


}

std::vector<dae::GameObject*> dae::EnemyComponent::GetPlayers()
{
	return m_Players;
}

bool dae::EnemyComponent::GetTethered()
{
	return m_Tethered;
}

void dae::EnemyComponent::SetTethered(bool value)
{
	m_Tethered = value;
}

int dae::EnemyComponent::GetValue()
{
	return m_Value;
}

bool dae::EnemyComponent::IncreaseInflation()
{
	++m_Inflation;
	if (m_Inflation >= m_MaxInflation) {
		GetOwner()->SetToBeDeleted(true);
		return true;
	}
	return false;
}

void dae::EnemyComponent::ResetInflation()
{
	m_Inflation = 0;
}
