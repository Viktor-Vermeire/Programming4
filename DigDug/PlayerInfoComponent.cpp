#include "PlayerInfoComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "PlayerComponent.h"
#include "HealthComponent.h"

dae::PlayerInfoComponent::PlayerInfoComponent(GameObject& gameObject): BaseComponent(gameObject)
{
}

void dae::PlayerInfoComponent::onNotify(GameObject& entity, Event event)
{
	TextComponent* text{};
	HealthComponent* healthComp{};
	PlayerComponent* playerComp;
	switch (event) {
	case DEATH:
		text = GetOwner()->GetComponent<TextComponent>();
		healthComp = entity.GetComponent<HealthComponent>();
		if (text && healthComp) {
			text->SetText("# lives: " + std::to_string(healthComp->GetLives()));
		}
		break;
	case SCORE:
		text = GetOwner()->GetComponent<TextComponent>();
		playerComp = entity.GetComponent<PlayerComponent>();
		if (text && playerComp) {
			text->SetText("Score: " + std::to_string(playerComp->GetScore()));
		}
		break;
	}
}
