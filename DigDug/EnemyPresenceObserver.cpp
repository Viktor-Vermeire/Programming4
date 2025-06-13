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
		m_Enemies = SceneManager::GetInstance().GetActiveScene()->findActiveGameObjectsWithComponent<dae::EnemyComponent>();

		/*auto it = result.begin();
		while ((it = std::find_if(it, result.end(), [](GameObject* gameObject) {
			return gameObject->m_Active;
			})) != result.end()) {
			m_Enemies.push_back(*it);
			++it;
		}*/
	}
}

std::vector<dae::GameObject*> dae::EnemyPresenceObserver::GetLivingEnemies()
{
	return m_Enemies;
}

void dae::EnemyPresenceObserver::PrepObserver(GameObject&, Event event, Scene& toCheckScene)
{
	if (event == SCORE)
		m_Enemies = toCheckScene.findActiveGameObjectsWithComponent<dae::EnemyComponent>();
}
