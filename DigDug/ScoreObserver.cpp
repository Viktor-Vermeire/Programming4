#include "ScoreObserver.h"
#include "GameObject.h"
#include "PlayerComponent.h"

dae::ScoreObserver::ScoreObserver()
{
	//m_Triggered = false;
	m_Score = 0;
}

void dae::ScoreObserver::onNotify(GameObject& gameObject, Event event)
{
	if (event == SCORE)
		m_Score = gameObject.GetComponent<dae::PlayerComponent>()->GetScore();
}

int dae::ScoreObserver::GetScore()
{
	return m_Score;
}
