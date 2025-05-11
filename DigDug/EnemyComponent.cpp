#include "EnemyComponent.h"
#include "GameObject.h"
#include "vector"

dae::EnemyComponent::EnemyComponent(GameObject& gameObject): BaseComponent(gameObject) {

}

void dae::EnemyComponent::AddPlayers(GameObject* gameObject)
{
	m_Players.emplace_back(gameObject);
}

std::vector<dae::GameObject*> dae::EnemyComponent::GetPlayers()
{
	return m_Players;
}
