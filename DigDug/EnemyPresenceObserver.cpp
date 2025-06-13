#include "EnemyPresenceObserver.h"
#include "SceneManager.h"
#include "Scene.h"
#include "EnemyComponent.h"
#include "algorithm"

dae::EnemyPresenceObserver::EnemyPresenceObserver()
{
}

void dae::EnemyPresenceObserver::onNotify(GameObject&, Event event)
{
	if (event == SCORE) {
		m_Enemies = {};
		std::vector<GameObject*> result = SceneManager::GetInstance().GetActiveScene()->findGameObjectsWithComponent<dae::EnemyComponent>();

		auto it = result.begin();
		while ((it = std::find_if(it, result.end(), [](GameObject* gameObject) {
			return !gameObject->IsToBeDeleted();
			})) != result.end()) {
			m_Enemies.push_back(*it);
			++it;
		}
	}
}

std::vector<dae::GameObject*> dae::EnemyPresenceObserver::GetLivingEnemies()
{
	return m_Enemies;
}

void dae::EnemyPresenceObserver::PrepObserver(GameObject&, Event event, Scene& toCheckScene)
{
	if (event == SCORE)
		m_Enemies = toCheckScene.findGameObjectsWithComponent<dae::EnemyComponent>();
}
