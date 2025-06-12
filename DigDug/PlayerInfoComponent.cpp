#include "PlayerInfoComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "PlayerComponent.h"
#include "HealthComponent.h"
#include "HealthObserver.h"
#include "ScoreObserver.h"

dae::PlayerInfoComponent::PlayerInfoComponent(GameObject& gameObject): BaseComponent(gameObject)
{
	m_ScoreContainer = std::make_unique<ScoreObserver>();
	m_HealthContainer = std::make_unique<HealthObserver>();
}

void dae::PlayerInfoComponent::Update()
{
	auto text = GetOwner()->GetComponent<dae::TextComponent>();
	if (text) {
		text->SetText("Score: " + std::to_string(m_ScoreContainer->GetScore()) + "Lives: " + std::to_string(m_HealthContainer->GetHealth()) );
	}
}

int dae::PlayerInfoComponent::GetScore()
{
	return m_ScoreContainer->GetScore();
}

int dae::PlayerInfoComponent::GetHealth()
{
	return m_HealthContainer->GetHealth();
}

dae::IObserver* dae::PlayerInfoComponent::GetScoreContainer()
{
	return m_ScoreContainer.get();
}

dae::IObserver* dae::PlayerInfoComponent::GetHealthContainer()
{
	return m_HealthContainer.get();
}

/*void dae::PlayerInfoComponent::onNotify(GameObject& entity, Event event)
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
}*/
