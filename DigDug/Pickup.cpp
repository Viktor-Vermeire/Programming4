#include "Pickup.h"
#include "GameObject.h"
#include "PlayerComponent.h"

dae::Pickup::Pickup(int inputValue, bool usingGamepad, int scoreValue) : Command(inputValue, usingGamepad), m_ScoreValue{scoreValue}
{
}

void dae::Pickup::execute(GameObject& gameObject)
{
	PlayerComponent* info = gameObject.GetComponent<PlayerComponent>();
	if (info) {
		info->SetScore(info->GetScore() + m_ScoreValue);
	}
}
