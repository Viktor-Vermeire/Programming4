#include "FygarAttackCommand.h"
#include "FygarAttackComponent.h"
#include "GameObject.h"
#include "ServiceLocator.h"

dae::FygarAttackCommand::FygarAttackCommand(unsigned int inputValue, bool usingGamepad) :Command(inputValue, usingGamepad)
{
}

void dae::FygarAttackCommand::execute(GameObject& gameObject)
{
	auto attack = gameObject.GetComponent<dae::FygarAttackComponent>();
	ServiceLocator::get_SoundSystem().PlayEffect(0, 1);
	if (attack)
		attack->SetActive(true);
}
