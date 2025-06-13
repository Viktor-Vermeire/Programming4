#include "Conditions.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include "HallwaysComponent.h"
#include "FloatingComponent.h"
#include "FiniteStateMachineComponent.h"
#include "iostream"
#include "DigDugAttackComponent.h"
#include "EnemyComponent.h"
#include "FygarAttackComponent.h"

bool dae::HasMovementInput::IsMet(GameObject* gameObject)
{
    auto movement = gameObject->GetComponent<MovementComponent>();
    if (movement) {
        return movement->WantsToMove();
    }
    return false;
}

dae::HasNearbyHallway::HasNearbyHallway(float timeFloating): m_MinTimeFloating{timeFloating}
{
}

bool dae::HasNearbyHallway::IsMet(GameObject* gameObject)
{
    auto comp = gameObject->GetComponent<FloatingComponent>();
    if (comp) {
        if (comp->GetTimeFloating() < m_MinTimeFloating)
            return false;
    }
	auto parent = gameObject->GetParent();
	if (parent) {
		auto hallways = parent->GetComponent<HallwaysComponent>();
		if (hallways) { //Rework this 
			if (hallways->GetFreeHallwayLocation(gameObject->GetWorldTransform().GetPosition()) != glm::vec3()) {
				return true;
			}
		}
	}
	return false;
}

bool dae::HasValidDirection::IsMet(GameObject* gameObject)
{
    auto comp = gameObject->GetComponent<RenderComponent>();
    auto parent = gameObject->GetParent();
    auto movement = gameObject->GetComponent<dae::MovementComponent>();
    if (comp && parent && movement) {
        auto hallways = parent->GetComponent<HallwaysComponent>();
        if (hallways) {
            dae::HallwaysComponent::HallwayType hallType;
            switch (comp->GetDirection()) {
            case RenderComponent::LEFT:
                hallType = hallways->GetHallwayType({ gameObject->GetWorldTransform().GetPosition().x - 16, gameObject->GetWorldTransform().GetPosition().y, 0 });
                if (hallType != HallwaysComponent::FILLED) {
                    auto wantsToMove = movement->WantsToMove();
                    movement->Move(comp->GetDirection());
                    auto valid = movement->WantsToMove();
                    movement->SetWantsToMove(wantsToMove);
                    return valid;
                }
                break;
            case RenderComponent::RIGHT:
                hallType = hallways->GetHallwayType({ gameObject->GetWorldTransform().GetPosition().x + 16, gameObject->GetWorldTransform().GetPosition().y, 0 });
                if (hallType != HallwaysComponent::FILLED) {
                    //::cout << "x:" << gameObject->GetWorldTransform().GetPosition().x + 16 << " y: " << gameObject->GetWorldTransform().GetPosition().y << "\n";
                    //std::cout << "type: " << hallType << "\n";
                    //std::cout << "RIGHT \n";
                    auto wantsToMove = movement->WantsToMove();
                    movement->Move(comp->GetDirection());
                    auto valid = movement->WantsToMove();
                    movement->SetWantsToMove(wantsToMove);
                    return valid;
                }
                break;
            case RenderComponent::UP:
                hallType = hallways->GetHallwayType({ gameObject->GetWorldTransform().GetPosition().x, gameObject->GetWorldTransform().GetPosition().y - 16, 0 });
                if (hallType != HallwaysComponent::FILLED) {
                    //std::cout << "x:" << gameObject->GetWorldTransform().GetPosition().x  << " y: " << gameObject->GetWorldTransform().GetPosition().y -16 << "\n";
                    //std::cout << "type: " << hallType << "\n";
                    //std::cout << "UP \n";
                    auto wantsToMove = movement->WantsToMove();
                    movement->Move(comp->GetDirection());
                    auto valid = movement->WantsToMove();
                    movement->SetWantsToMove(wantsToMove);
                    return valid;
                }
                break;
            case RenderComponent::DOWN:
                hallType = hallways->GetHallwayType({ gameObject->GetWorldTransform().GetPosition().x, gameObject->GetWorldTransform().GetPosition().y + 16, 0 });
                if (hallType != HallwaysComponent::FILLED) {
                    //::cout << "x:" << gameObject->GetWorldTransform().GetPosition().x << " y: " << gameObject->GetWorldTransform().GetPosition().y +16 << "\n";
                    //std::cout << "type: " << hallType << "\n";
                    //std::cout << "DOWN \n";
                    auto wantsToMove = movement->WantsToMove();
                    movement->Move(comp->GetDirection());
                    auto valid = movement->WantsToMove();
                    movement->SetWantsToMove(wantsToMove);
                    return valid;
                }
                break;
            }
        }
    }
    return false;
}

bool dae::IsDoneRunning::IsMet(GameObject* gameObject)
{
    auto movement = gameObject->GetComponent<MovementComponent>();
    if (movement) {
        return !movement->IsRunning();
    }
    return false;
}

dae::WantsToFloat::WantsToFloat(float time) : m_MaxTime{ time }
{
}

bool dae::WantsToFloat::IsMet(GameObject* gameObject)
{
    auto comp = gameObject->GetComponent<FiniteStateMachineComponent>();
    if (comp) {
        if (comp->GetTimeInCurrentSituation() > m_MaxTime)
            return true;
        else
            return false;
    }
    return false;
}

bool dae::IsInGrid::IsMet(GameObject* gameObject)
{
    auto comp = gameObject->GetComponent<FloatingComponent>();
    if (comp)
        return comp->InGrid();
    return false;
}

bool dae::WantsToAttack::IsMet(GameObject* gameObject)
{
    auto comp = gameObject->GetComponent<DigDugAttackComponent>();
    if (comp)
        return comp->IsActive();
    return false;
}

bool dae::FinishedAttack::IsMet(GameObject* gameObject)
{
    auto comp = gameObject->GetComponent<DigDugAttackComponent>();
    if (comp)
        return !comp->IsActive();
    return false;
}

bool dae::IsTethered::IsMet(GameObject* gameObject)
{
    auto enemy = gameObject->GetComponent<dae::EnemyComponent>();
    if (enemy) {
        return enemy->GetTethered();
    }
    return false;
}

bool dae::IsUntethered::IsMet(GameObject* gameObject)
{
    auto enemy = gameObject->GetComponent<dae::EnemyComponent>();
    if (enemy) {
        return !enemy->GetTethered();
    }
    return false;
}

bool dae::FygarWantsToAttack::IsMet(GameObject* gameObject)
{
    auto comp = gameObject->GetComponent<FygarAttackComponent>();
    if (comp)
        return comp->IsActive();
    return false;
}

bool dae::FygarFinishedAttack::IsMet(GameObject* gameObject)
{
    auto comp = gameObject->GetComponent<FygarAttackComponent>();
    if (comp)
        return !comp->IsActive();
    return false;
}

bool dae::HasValidDirectionAndWantsToMove::IsMet(GameObject* gameObject)
{
    auto comp = gameObject->GetComponent<RenderComponent>();
    auto parent = gameObject->GetParent();
    auto movement = gameObject->GetComponent<dae::MovementComponent>();
    if (comp && parent && movement) {
        auto hallways = parent->GetComponent<HallwaysComponent>();
        if (hallways) {
            dae::HallwaysComponent::HallwayType hallType;
            switch (comp->GetDirection()) {
            case RenderComponent::LEFT:
                hallType = hallways->GetHallwayType({ gameObject->GetWorldTransform().GetPosition().x - 16, gameObject->GetWorldTransform().GetPosition().y, 0 });
                if (hallType != HallwaysComponent::FILLED) {
                    auto wantsToMove = movement->WantsToMove();
                    movement->Move(comp->GetDirection());
                    auto valid = movement->WantsToMove();
                    movement->SetWantsToMove(wantsToMove);
                    return (valid && wantsToMove);
                }
                break;
            case RenderComponent::RIGHT:
                hallType = hallways->GetHallwayType({ gameObject->GetWorldTransform().GetPosition().x + 16, gameObject->GetWorldTransform().GetPosition().y, 0 });
                if (hallType != HallwaysComponent::FILLED) {
                    auto wantsToMove = movement->WantsToMove();
                    movement->Move(comp->GetDirection());
                    auto valid = movement->WantsToMove();
                    movement->SetWantsToMove(wantsToMove);
                    return (valid && wantsToMove);
                }
                break;
            case RenderComponent::UP:
                hallType = hallways->GetHallwayType({ gameObject->GetWorldTransform().GetPosition().x, gameObject->GetWorldTransform().GetPosition().y - 16, 0 });
                if (hallType != HallwaysComponent::FILLED) {
                    auto wantsToMove = movement->WantsToMove();
                    movement->Move(comp->GetDirection());
                    auto valid = movement->WantsToMove();
                    movement->SetWantsToMove(wantsToMove);
                    return (valid && wantsToMove);
                }
                break;
            case RenderComponent::DOWN:
                hallType = hallways->GetHallwayType({ gameObject->GetWorldTransform().GetPosition().x, gameObject->GetWorldTransform().GetPosition().y + 16, 0 });
                if (hallType != HallwaysComponent::FILLED) {
                    auto wantsToMove = movement->WantsToMove();
                    movement->Move(comp->GetDirection());
                    auto valid = movement->WantsToMove();
                    movement->SetWantsToMove(wantsToMove);
                    return (valid && wantsToMove);
                }
                break;
            }
        }
    }
    return false;
}
