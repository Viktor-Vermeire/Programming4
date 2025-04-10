#include "PlayerComponent.h"
#include "BaseComponent.h"
#include "GameObject.h"
using namespace dae;


dae::PlayerComponent::PlayerComponent(GameObject& gameObject,std::string name) :
	BaseComponent(gameObject), m_Name{ name }, m_Score{0}
{
}

std::string dae::PlayerComponent::GetName()
{
	return m_Name;
}

int dae::PlayerComponent::GetScore()
{
	return m_Score;
}

void dae::PlayerComponent::SetScore(int score)
{
	m_Score = score;
	Notify(SCORE);
}

void dae::PlayerComponent::AddObserver(IObserver* observer)
{
	m_Observers.emplace_back(observer);
}

void dae::PlayerComponent::RemoveObserver(IObserver* observer)
{
	for (int looper{ 0 }; looper < m_Observers.size(); ++looper) {
		if (m_Observers.at(looper) != observer)
			continue;
		m_Observers.at(looper) = nullptr;
		m_Observers.erase(m_Observers.begin() + looper);
	}
}

void dae::PlayerComponent::Notify(Event event)
{
	for (auto observer : m_Observers) {
		observer->onNotify(*GetOwner(), event);
	}
}
