#include "DigDugAttackCommand.h"
#include "DigDugAttackComponent.h"
#include "GameObject.h"
#include "ServiceLocator.h"

dae::DigDugAttackCommand::DigDugAttackCommand(unsigned int inputValue, bool usingGamepad) :Command(inputValue, usingGamepad)
{

}

void dae::DigDugAttackCommand::execute(GameObject& gameObject)
{
	auto attack = gameObject.GetComponent<dae::DigDugAttackComponent>();
	ServiceLocator::get_SoundSystem().PlayEffect(0, 1);
	if (attack)
		attack->SetActive(true);
}
