#include "States.h"
#include "Minigin.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "FloatingComponent.h"
#include "MovementComponent.h"
#include "InputManager.h"
#include "HallwaysComponent.h"
#include "FiniteStateMachineComponent.h"
#include "DigDugAttackComponent.h"
#include "Command.h"
#include "Gamepad.h"

void dae::Attacking::Enter(GameObject*)
{
}

void dae::Attacking::Execute(GameObject*)
{
}

void dae::Attacking::Exit(GameObject*)
{
}

dae::FloatingToPlayer::FloatingToPlayer(std::vector<SDL_Rect> animationLocations, float timePerFrame) :
	m_AnimationLocations{ animationLocations }, m_TimePerFrame{ timePerFrame }
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
			else if (
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
	if (comp) {
		// initialise it setperate and then cast
		auto result = fmodf(m_TimeInState, m_TimePerFrame);
		auto castedtimeperframe = ((m_TimeInState - (result)) / m_TimePerFrame);
		auto check = static_cast<_int64>(castedtimeperframe);
		comp->SetBox(m_AnimationLocations[check]);
	}

	//Movement Part
	auto floating = gameObject->GetComponent<FloatingComponent>();
	if (floating) {
		floating->MoveTarget();
	}

}

void dae::FloatingToPlayer::Exit(GameObject*)
{
}

float dae::FloatingToPlayer::GetTimeInState()
{
	return m_TimeInState;
}

dae::FloatingToGrid::FloatingToGrid(std::vector<SDL_Rect> animationLocations, float timePerFrame) :
	m_AnimationLocations{ animationLocations }, m_TimePerFrame{ timePerFrame }
{
}

void dae::FloatingToGrid::Enter(GameObject* gameObject)
{
	auto parent = gameObject->GetParent();
	auto floating = gameObject->GetComponent<FloatingComponent>();
	if (parent && floating) {
		auto hallways = parent->GetComponent<HallwaysComponent>();
		if (hallways) { //Rework this 
			floating->SetGridTarget(hallways->GetFreeHallwayLocation(gameObject->GetWorldTransform().GetPosition()));
			m_Target = hallways->GetFreeHallwayLocation(gameObject->GetWorldTransform().GetPosition());
			floating->SetInGrid(false);
		}
	}
	//To be Filled In ESSENTIAL
}

void dae::FloatingToGrid::Execute(GameObject* gameObject)
{
	//AnimationPart
	m_TimeInState += Minigin::DELTATIME;
	if (m_TimeInState > m_AnimationLocations.size() * m_TimePerFrame)
		m_TimeInState -= (m_AnimationLocations.size() * m_TimePerFrame);
	auto comp = gameObject->GetComponent<RenderComponent>();
	if (comp) {
		auto result = fmodf(m_TimeInState, m_TimePerFrame);
		auto castedtimeperframe = ((m_TimeInState - (result)) / m_TimePerFrame);
		auto check = static_cast<_int64>(castedtimeperframe);
		comp->SetBox(m_AnimationLocations[check]);
	}
	//Movement Part
	auto floating = gameObject->GetComponent<FloatingComponent>();
	if (floating) {
		floating->MoveToGrid();
	}
}

void dae::FloatingToGrid::Exit(GameObject* gameObject)
{
	auto comp = gameObject->GetComponent<FiniteStateMachineComponent>();
	if (comp)
		comp->SetTimeInCurrentSituation(0.f);
}

void dae::Idle::Enter(GameObject* gameObject)
{
	auto parent = gameObject->GetParent();
	if (parent) {
		auto hall = parent->GetComponent<dae::HallwaysComponent>();
		if (hall) {
			auto pos = hall->GetFreeHallwayLocation(gameObject->GetWorldTransform().GetPosition());
			gameObject->SetPosition(pos.x, pos.y);
		}
	}
}

void dae::Idle::Execute(GameObject* gameObject)
{
	std::uniform_int_distribution<int> dist(0, 3);
	auto comp = gameObject->GetComponent<RenderComponent>();
	if (comp) {
		switch (dist(Minigin::RANDOM)) {
		case 0:
			comp->SetDirection(RenderComponent::RIGHT);
			return;
		case 1:
			comp->SetDirection(RenderComponent::LEFT);
			return;
		case 2:
			comp->SetDirection(RenderComponent::UP);
			return;
		case 3:
			comp->SetDirection(RenderComponent::DOWN);
			return;
		}
		;
	}
}

void dae::Idle::Exit(GameObject*)
{
}

void dae::Running::Enter(GameObject* gameObject)
{
	auto movement = gameObject->GetComponent<MovementComponent>();
	auto render = gameObject->GetComponent<RenderComponent>();
	if (movement != nullptr && render != nullptr) {
		movement->ResetMovement();
		movement->Move(render->GetDirection());
	}
}
void dae::Running::Execute(GameObject* gameObject)
{
	auto movement = gameObject->GetComponent<MovementComponent>();
	if (movement != nullptr) {
		movement->ExecuteMove();
	}
}

void dae::Running::Exit(GameObject* gameObject)
{
	auto movement = gameObject->GetComponent<MovementComponent>();
	if (movement != nullptr) {
		movement->SetWantsToMove(false);
	}
}

dae::DigDugAttack::DigDugAttack(Gamepad* gamepad)
{
	m_Gamepad = gamepad;
}



void dae::DigDugAttack::Enter(GameObject* gameObject)
{
	auto attack = gameObject->GetComponent<dae::DigDugAttackComponent>();
	if (attack) {
		attack->SetGrowing(true);
		attack->SetTimeGrowing(0.f);
	}
}

void dae::DigDugAttack::Execute(GameObject* gameobject)
{
	auto comp = gameobject->GetComponent<dae::DigDugAttackComponent>();
	if (comp)
		comp->LowerCoolDown();
	InputManager::GetInstance().ProcessPlayerInput(gameobject, m_Gamepad, m_Commands);
}

void dae::DigDugAttack::Exit(GameObject*)
{
}

void dae::DigDugAttack::AddCommand(Command* command)
{
	m_Commands.emplace_back(command);
}

void dae::Tethered::Enter(GameObject*)
{
}

void dae::Tethered::Execute(GameObject*)
{
}

void dae::Tethered::Exit(GameObject*)
{
}
