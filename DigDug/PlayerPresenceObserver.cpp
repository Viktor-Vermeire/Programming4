#include "PlayerPresenceObserver.h"
#include "SceneManager.h"
#include "Scene.h"
#include "PlayerComponent.h"
#include "HealthComponent.h"
#include "GameObject.h"
#include "iostream"
dae::PlayerPresenceObserver::PlayerPresenceObserver()
{
}
void dae::PlayerPresenceObserver::onNotify(GameObject&, Event event)
{
	if (event == DEATH) {
		m_LivingPlayers = {};
		for (auto go : SceneManager::GetInstance().GetActiveScene()->findGameObjectsWithComponent<PlayerComponent>()) {
			auto health = go->GetComponent<HealthComponent>();
			if (health) {
				if (health->GetLives() > 0)
					m_LivingPlayers.emplace_back(go);
			}

		};
	}
}

void dae::PlayerPresenceObserver::PrepObserver(GameObject&, Event event, Scene& toCheckScene)
{
	if (event == DEATH) {
		m_LivingPlayers = {};
		for (auto go : toCheckScene.findGameObjectsWithComponent<PlayerComponent>()) {
			auto health = go->GetComponent<HealthComponent>();
			if (health) {
				if (health->GetLives() > 0)
					m_LivingPlayers.emplace_back(go);
			}

		};
	}
}

std::vector<dae::GameObject*> dae::PlayerPresenceObserver::GetLivingPlayers()
{
	return m_LivingPlayers;
}
