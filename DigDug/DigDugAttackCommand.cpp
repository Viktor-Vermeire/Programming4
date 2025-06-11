#include "DigDugAttackCommand.h"
#include "DigDugAttackComponent.h"
#include "GameObject.h"

dae::DigDugAttackCommand::DigDugAttackCommand(int inputValue, bool usingGamepad) :Command(inputValue, usingGamepad)
{

}

void dae::DigDugAttackCommand::execute(GameObject& gameObject)
{
	auto attack = gameObject.GetComponent<dae::DigDugAttackComponent>();
	if (attack)
		attack->SetActive(true);
}
