#include "Suicide.h"
#include "GameObject.h"
#include "HealthComponent.h"

dae::Suicide::Suicide(int inputValue, bool usingGamepad) : Command(inputValue, usingGamepad)
{
}

void dae::Suicide::execute(GameObject& gameObject)
{
	HealthComponent* healthComp = gameObject.GetComponent<HealthComponent>();
	if (healthComp) {
		healthComp->Die();
	}
}
