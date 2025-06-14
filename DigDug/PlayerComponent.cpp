#include "PlayerComponent.h"
#include "BaseComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "EnemyComponent.h"
#include "HealthComponent.h"

dae::PlayerComponent::PlayerComponent(GameObject& gameObject,const std::string& name) :
	BaseComponent(gameObject), m_Name{ name }, m_Score{0}
{
}

std::string& dae::PlayerComponent::GetName()
{
	return m_Name;
}

void dae::PlayerComponent::Update()
{
	auto enemies = dae::SceneManager::GetInstance().GetActiveScene()->findActiveGameObjectsWithComponent<dae::EnemyComponent>();
	auto result = std::find_if(enemies.begin(), enemies.end(), [&](GameObject* gameObject) {
		auto render = GetOwner()->GetComponent<dae::RenderComponent>();
		auto enemyRender = gameObject->GetComponent<dae::RenderComponent>();
		if (render && enemyRender) {
			auto enPos = gameObject->GetWorldTransform().GetPosition();
			auto plPos = GetOwner()->GetWorldTransform().GetPosition();
			auto rect = SDL_Rect{ static_cast<int>(enPos.x), static_cast<int>(enPos.y), enemyRender->GetBox().w, enemyRender->GetBox().h };
			auto plRect = SDL_Rect{ static_cast<int>(plPos.x), static_cast<int>(plPos.y), render->GetBox().w, render->GetBox().h };
			return SDL_HasIntersection(&rect, &plRect);
		}
		return SDL_FALSE;
		});
	if (result != enemies.end()) {
		auto health = GetOwner()->GetComponent<HealthComponent>();
		if (health) {
			health->Die();
		}
		//set Playerdamage logic here
	}
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
