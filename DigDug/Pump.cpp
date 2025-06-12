#include "Pump.h"
#include "GameObject.h"
#include "DigDugAttackComponent.h"

dae::Pump::Pump(int inputValue, bool usingGamepad): Command(inputValue, usingGamepad)
{
}

void dae::Pump::execute(GameObject& gameObject)
{
	auto attack = gameObject.GetComponent<dae::DigDugAttackComponent>();
	if (attack)
		attack->Pump();
}
