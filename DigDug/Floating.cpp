#include "Floating.h"
#include "Minigin.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "FloatingComponent.h"
#include "InputManager.h"

dae::FloatingToPlayer::FloatingToPlayer(std::vector<SDL_Rect> animationLocations, float timePerFrame): 
	m_AnimationLocations{animationLocations}, m_TimePerFrame{timePerFrame}
{
}

void dae::FloatingToPlayer::Enter(GameObject* gameObject)
{
	m_TimeInState = 0;
	auto floating = gameObject->GetComponent<FloatingComponent>();
	if (floating) {
		GameObject* closestPlayer = nullptr;
		for (auto player : InputManager::GetInstance().GetPlayers()) {
			if (closestPlayer == nullptr)
				closestPlayer = player;
			else if(
				glm::distance(gameObject->GetWorldTransform().GetPosition(),
					player->GetWorldTransform().GetPosition()) <
				glm::distance(gameObject->GetWorldTransform().GetPosition(),
					closestPlayer->GetWorldTransform().GetPosition()))
				 {
				closestPlayer = player;
			}
		}
		floating->SetPlayerTarget(closestPlayer);
	}
}

void dae::FloatingToPlayer::Execute(GameObject* gameObject)
{
	//AnimationPart
	m_TimeInState += Minigin::DELTATIME;
	if (m_TimeInState > m_AnimationLocations.size() * m_TimePerFrame)
		m_TimeInState -= (m_AnimationLocations.size() * m_TimePerFrame);
	auto comp = gameObject->GetComponent<RenderComponent>();
	if (comp)
		comp->SetBox(m_AnimationLocations[static_cast<_int64>(m_TimeInState - (fmodf(m_TimeInState, m_TimePerFrame))) / m_TimePerFrame]);

	//Movement Part
	auto floating = gameObject->GetComponent<FloatingComponent>();
	if (floating) {
		floating->MoveTarget();
	}

}

void dae::FloatingToPlayer::Exit(GameObject*)
{
}

dae::FloatingToGrid::FloatingToGrid(std::vector<SDL_Rect> animationLocations, float timePerFrame):
	m_AnimationLocations{ animationLocations }, m_TimePerFrame{ timePerFrame }
{
}

void dae::FloatingToGrid::Enter(GameObject*)
{
	//To be Filled In ESSENTIAL
}

void dae::FloatingToGrid::Execute(GameObject* gameObject)
{
	//AnimationPart
	m_TimeInState += Minigin::DELTATIME;
	if (m_TimeInState > m_AnimationLocations.size() * m_TimePerFrame)
		m_TimeInState -= (m_AnimationLocations.size() * m_TimePerFrame);
	auto comp = gameObject->GetComponent<RenderComponent>();
	if (comp)
		comp->SetBox(m_AnimationLocations[static_cast<_int64>(m_TimeInState - (fmodf(m_TimeInState, m_TimePerFrame))) / m_TimePerFrame]);
	//Movement Part
	auto floating = gameObject->GetComponent<FloatingComponent>();
	if (floating) {
		floating->MoveToGrid();
	}
}

void dae::FloatingToGrid::Exit(GameObject*)
{
}
