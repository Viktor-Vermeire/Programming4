#include "GameMasterComponent.h"
#include "EnemyPresenceObserver.h"
#include "PlayerPresenceObserver.h"
#include "iostream"
#include "GameObject.h"
#include "SceneManager.h"
#include "IObserver.h"
#include "Scene.h"
dae::GameMasterComponent::GameMasterComponent(GameObject& gameobject, std::string succesScene, std::string loseScene): 
	BaseComponent(gameobject), m_SuccesScene{succesScene}, m_LoseScene{loseScene}
{
	m_EnemyPresenceObserver = std::make_unique<EnemyPresenceObserver>();
	m_PlayerPresenceObserver = std::make_unique<PlayerPresenceObserver>();
}

void dae::GameMasterComponent::Update()
{
	if (m_EnemyPresenceObserver->GetLivingEnemies().empty()) {
		SceneManager::GetInstance().SetToSwitchScene(*SceneManager::GetInstance().GetScene(m_SuccesScene));
		return;
	}
	if (m_PlayerPresenceObserver->GetLivingPlayers().empty()) {
		SceneManager::GetInstance().SetToSwitchScene(*SceneManager::GetInstance().GetScene(m_LoseScene));
		return;
	}
}

dae::IObserver* dae::GameMasterComponent::GetPlayerPresenceObserver()
{
	return m_PlayerPresenceObserver.get();
}

dae::IObserver* dae::GameMasterComponent::GetEnemyPresenceObserver()
{
	return m_EnemyPresenceObserver.get();
}

void dae::GameMasterComponent::PrepGameMaster(Scene& scene)
{
	m_EnemyPresenceObserver->PrepObserver(*GetOwner(), SCORE, scene);
	m_PlayerPresenceObserver->PrepObserver(*GetOwner(), DEATH, scene);
}
