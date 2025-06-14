#include "GameMasterComponent.h"
#include "EnemyPresenceObserver.h"
#include "PlayerPresenceObserver.h"
#include "iostream"
#include "GameObject.h"
#include "SceneManager.h"
#include "IObserver.h"
#include "Scene.h"
#include "Minigin.h"
dae::GameMasterComponent::GameMasterComponent(GameObject& gameobject,const std::string& succesScene,const std::string& loseScene): 
	BaseComponent(gameobject), m_SuccesScene{succesScene}, m_LoseScene{loseScene}
{
	m_EnemyPresenceObserver = std::make_unique<EnemyPresenceObserver>();
	m_PlayerPresenceObserver = std::make_unique<PlayerPresenceObserver>();
	m_StartUpCooldown = 2;
	m_RemainingCooldown = m_StartUpCooldown;
}

void dae::GameMasterComponent::Update()
{
	if (m_RemainingCooldown > 0) 
		m_RemainingCooldown -= Minigin::DELTATIME;
	if (m_EnemyPresenceObserver->GetLivingEnemies().empty() || m_ToBeSkipped) {
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

void dae::GameMasterComponent::SetToBeSkipped(bool value)
{
	if (m_RemainingCooldown > 0) return;
	m_ToBeSkipped = value;
}
