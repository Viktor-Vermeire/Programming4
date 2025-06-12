#include "DigDugAttackComponent.h"
#include "Minigin.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "iostream"
#include "SceneManager.h"
#include "Scene.h"
#include "EnemyComponent.h"
#include "PlayerComponent.h"
#include "ServiceLocator.h"

dae::DigDugAttackComponent::DigDugAttackComponent(GameObject& go, SDL_Rect maxAttackRect, std::string texturePath, float maxGrowTime, float pumpCooldown)
	: BaseComponent(go), 
m_MaxAttackRect{maxAttackRect}, m_MaxGrowTime{maxGrowTime}, m_PumpCooldown{pumpCooldown}
{
	m_Active = false;
	m_TimeGrowing = 0;
	m_Growing = false;
	m_Connected = false;
	m_Texture = ResourceManager::GetInstance().LoadTexture(texturePath).get();
	
	auto render = GetOwner()->GetComponent<RenderComponent>();
	if (render)
		m_SpriteXYOffset = { render->GetBox().w, render->GetBox().h };
	m_CurrentAttackRect = SDL_Rect(maxAttackRect.x + maxAttackRect.w, maxAttackRect.y, 0, maxAttackRect.h);
}

bool dae::DigDugAttackComponent::IsActive()
{
	return m_Active;
}

void dae::DigDugAttackComponent::SetActive(bool active)
{
	m_Active = active;
}

void dae::DigDugAttackComponent::SetGrowing(bool growing)
{
	m_Growing = growing;
}

void dae::DigDugAttackComponent::SetTimeGrowing(float time)
{
	m_TimeGrowing = time;
}

void dae::DigDugAttackComponent::Pump()
{
	if (m_CurrentCooldown <= 0 && m_Connected) {
		ServiceLocator::get_SoundSystem().PlayEffect(1,2.f);
		m_CurrentCooldown = m_PumpCooldown;
		auto comp = m_TetheredEnemy->GetComponent<EnemyComponent>();
		auto score = GetOwner()->GetComponent<PlayerComponent>();
		if (comp) {
			if (comp->IncreaseInflation()) {
				m_Connected = false;
				m_Active = false;
				m_TimeGrowing = 0.f;
				if (score)
					score->SetScore(score->GetScore() + comp->GetValue());
			}
		}
		std::cout << "Pumped \n";
	}
}

void dae::DigDugAttackComponent::LowerCoolDown()
{
	if (m_CurrentCooldown > 0)
		m_CurrentCooldown -= Minigin::DELTATIME;
}

void dae::DigDugAttackComponent::Update()
{
	if (!m_Active) return;

	if (m_Growing) {
		m_TimeGrowing += Minigin::DELTATIME;
		//std::cout << "time passed: " << m_TimeGrowing << "\n max Time: " << m_MaxGrowTime << "\n";
		if (m_TimeGrowing > m_MaxGrowTime) {
			m_Growing = false;
			m_TimeGrowing = m_MaxGrowTime;
		}
		m_CurrentAttackRect.w = static_cast<int>(std::roundf(m_MaxAttackRect.w * (m_TimeGrowing / m_MaxGrowTime)));
		m_CurrentAttackRect.x = (m_MaxAttackRect.x + m_MaxAttackRect.w - m_CurrentAttackRect.w);
	}
	if (!m_Connected) {
		auto enemies = dae::SceneManager::GetInstance().GetActiveScene()->findGameObjectsWithComponent<dae::EnemyComponent>();
		auto result = std::find_if(enemies.begin(), enemies.end(), [&](GameObject* gameObject) {
			auto render = gameObject->GetComponent<dae::RenderComponent>();
			if (render) {
				auto enPos = gameObject->GetWorldTransform().GetPosition();
				auto plPos = GetOwner()->GetWorldTransform().GetPosition();
				auto rect = SDL_Rect{ static_cast<int>(enPos.x), static_cast<int>(enPos.y), render->GetBox().w, render->GetBox().h };
				auto plRect = GetWorldSpikeRect(*render);
				return SDL_HasIntersection(&rect, &plRect);
			}
			return SDL_FALSE;
			});
		if (result != enemies.end()) {
			m_Connected = true;
			m_Growing = false;
			m_TetheredEnemy = *result;
			(*result)->GetComponent<dae::EnemyComponent>()->SetTethered(true);
		}
	}
	else {
		if (!m_TetheredEnemy->GetComponent<dae::EnemyComponent>()->GetTethered()) {
			m_Connected = false;
			m_Active = false;
			m_TimeGrowing = 0.f;
			m_TetheredEnemy = nullptr;
		}
	}

	if (!m_Growing && !m_Connected) {
		m_Active = false;
		m_TimeGrowing = 0;
	}
}



void dae::DigDugAttackComponent::Render()
{
	if (m_Active) {
		auto spriteRenderer = BaseComponent::GetOwner()->GetComponent<RenderComponent>();
		if (spriteRenderer) {
			//const auto& pos = BaseComponent::GetOwner()->GetWorldTransform().GetPosition();
			switch (spriteRenderer->GetDirection()) {
			case RenderComponent::RIGHT:
				Renderer::GetInstance().RenderTexture(*m_Texture, 
					SDL_Rect(m_CurrentAttackRect.x, m_CurrentAttackRect.y, m_CurrentAttackRect.w, m_CurrentAttackRect.h),
					GetWorldSpikeRect(*spriteRenderer));
				break;
			case RenderComponent::LEFT:
				Renderer::GetInstance().RenderTextureOriented(*m_Texture, 
					SDL_Rect(m_CurrentAttackRect.x, m_CurrentAttackRect.y, m_CurrentAttackRect.w, m_CurrentAttackRect.h),
					GetWorldSpikeRect(*spriteRenderer),
					180, SDL_FLIP_VERTICAL, false);
				break;
			case RenderComponent::UP:
				Renderer::GetInstance().RenderTextureOriented(*m_Texture, 
					SDL_Rect(m_CurrentAttackRect.x, m_CurrentAttackRect.y, m_CurrentAttackRect.w, m_CurrentAttackRect.h),
					GetWorldSpikeRect(*spriteRenderer),
					-90, SDL_FLIP_NONE, false);
				break;
			case RenderComponent::DOWN:
				Renderer::GetInstance().RenderTextureOriented(*m_Texture, 
					SDL_Rect(m_CurrentAttackRect.x, m_CurrentAttackRect.y, m_CurrentAttackRect.w, m_CurrentAttackRect.h),
					GetWorldSpikeRect(*spriteRenderer),
					90, SDL_FLIP_NONE, false);
				break;
			}
		}
	}
}

SDL_Rect dae::DigDugAttackComponent::GetWorldSpikeRect(RenderComponent& render)
{
		const auto& pos = BaseComponent::GetOwner()->GetWorldTransform().GetPosition();
		switch (render.GetDirection()) {
		case RenderComponent::RIGHT:

			return SDL_Rect(static_cast<int>(pos.x) + m_SpriteXYOffset.first,
				static_cast<int>(pos.y) + m_SpriteXYOffset.second / 2 - m_CurrentAttackRect.h / 2,
				m_CurrentAttackRect.w, m_CurrentAttackRect.h);
			break;
		case RenderComponent::LEFT:
			return SDL_Rect(static_cast<int>(pos.x),
				static_cast<int>(pos.y) + m_CurrentAttackRect.h + m_SpriteXYOffset.second / 2 - m_CurrentAttackRect.h / 2,
				m_CurrentAttackRect.w, m_CurrentAttackRect.h);
			break;
		case RenderComponent::UP:
			return SDL_Rect(static_cast<int>(pos.x) + m_SpriteXYOffset.second / 2 - m_CurrentAttackRect.h / 2,
				static_cast<int>(pos.y),
				m_CurrentAttackRect.w, m_CurrentAttackRect.h);
			break;
		case RenderComponent::DOWN:
			return SDL_Rect(static_cast<int>(pos.x) + m_SpriteXYOffset.first - m_SpriteXYOffset.second / 2 + m_CurrentAttackRect.h / 2,
				static_cast<int>(pos.y) + m_SpriteXYOffset.first,
				m_CurrentAttackRect.w, m_CurrentAttackRect.h);
			break;
		}
		return {};
}
