#include "FygarAttackComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "Minigin.h"
#include "SceneManager.h"
#include "PlayerComponent.h"
#include "Scene.h"
#include "iostream"
#include "HealthComponent.h"
#include "Renderer.h"

dae::FygarAttackComponent::FygarAttackComponent(GameObject& gameObject,const std::vector<SDL_Rect>& flameRects, 
	const std::string& texturePath, float maxFlameTime): BaseComponent(gameObject), m_FlameRects{flameRects}, m_MaxFlameTime{maxFlameTime}
{
	m_Active = false;
	m_CurrentFlameRect = {};
	m_TimeFlaming = 0.f;
	auto render = GetOwner()->GetComponent<RenderComponent>();
	if (render)
		m_SpriteXYOffset = { render->GetBox().w, render->GetBox().h };
	m_Texture = ResourceManager::GetInstance().LoadTexture(texturePath).get();
}

bool dae::FygarAttackComponent::IsActive()
{
	return m_Active;
}

void dae::FygarAttackComponent::SetActive(bool active)
{
	m_Active = active;
}

void dae::FygarAttackComponent::Update()
{
	if (!m_Active) return;

	m_TimeFlaming += Minigin::DELTATIME;

	auto timePerFlame = m_MaxFlameTime / m_FlameRects.size();
	auto rest = std::fmodf(m_TimeFlaming, timePerFlame);
	auto index = static_cast<_int64>((m_TimeFlaming - rest) / timePerFlame);
	
	if (m_TimeFlaming > m_MaxFlameTime) {
		m_Active = false;
		m_TimeFlaming = 0;
		m_CurrentFlameRect = {};
	}
	else {
		m_CurrentFlameRect = m_FlameRects[index];
	}

	auto players = dae::SceneManager::GetInstance().GetActiveScene()->findActiveGameObjectsWithComponent<dae::PlayerComponent>();
	auto result = std::find_if(players.begin(), players.end(), [&](GameObject* gameObject) {
		auto render = GetOwner()->GetComponent<dae::RenderComponent>();
		auto enemyRender = gameObject->GetComponent<dae::RenderComponent>();
		if (render && enemyRender) {
			auto enPos = gameObject->GetWorldTransform().GetPosition();
			auto plPos = GetOwner()->GetWorldTransform().GetPosition();
			auto rect = SDL_Rect{ static_cast<int>(enPos.x), static_cast<int>(enPos.y), enemyRender->GetBox().w, enemyRender->GetBox().h };
			auto plRect = GetWorldFlameRectCollision(*render);
			return SDL_HasIntersection(&rect, &plRect);
		}
		return SDL_FALSE;
		});
	if (result != players.end()) {
		auto health = (*result)->GetComponent<HealthComponent>();
		if (health) {
			health->Die();
		}
		//set Playerdamage logic here
	}
}

void dae::FygarAttackComponent::Render()
{
	if (m_Active) {
		auto spriteRenderer = BaseComponent::GetOwner()->GetComponent<RenderComponent>();
		if (spriteRenderer) {
			//const auto& pos = BaseComponent::GetOwner()->GetWorldTransform().GetPosition();
			switch (spriteRenderer->GetDirection()) {
			case RenderComponent::RIGHT:
				
				Renderer::GetInstance().RenderTexture(*m_Texture,
					SDL_Rect(m_CurrentFlameRect.x, m_CurrentFlameRect.y, m_CurrentFlameRect.w, m_CurrentFlameRect.h),
					GetWorldFlameRect(*spriteRenderer));
				break;
			case RenderComponent::LEFT:
				Renderer::GetInstance().RenderTextureOriented(*m_Texture,
					SDL_Rect(m_CurrentFlameRect.x, m_CurrentFlameRect.y, m_CurrentFlameRect.w, m_CurrentFlameRect.h),
					GetWorldFlameRect(*spriteRenderer),
					180, SDL_FLIP_VERTICAL, false);
				break;
			case RenderComponent::UP:
				Renderer::GetInstance().RenderTextureOriented(*m_Texture,
					SDL_Rect(m_CurrentFlameRect.x, m_CurrentFlameRect.y, m_CurrentFlameRect.w, m_CurrentFlameRect.h),
					GetWorldFlameRect(*spriteRenderer),
					-90, SDL_FLIP_NONE, false);
				break;
			case RenderComponent::DOWN:
				Renderer::GetInstance().RenderTextureOriented(*m_Texture,
					SDL_Rect(m_CurrentFlameRect.x, m_CurrentFlameRect.y, m_CurrentFlameRect.w, m_CurrentFlameRect.h),
					GetWorldFlameRect(*spriteRenderer),
					90, SDL_FLIP_NONE, false);
				break;
			}
		}
	}
}

SDL_Rect dae::FygarAttackComponent::GetWorldFlameRect(RenderComponent& render)
{
	const auto& pos = BaseComponent::GetOwner()->GetWorldTransform().GetPosition();
	switch (render.GetDirection()) {
	case RenderComponent::RIGHT:
		return SDL_Rect(static_cast<int>(pos.x) + m_SpriteXYOffset.first,
			static_cast<int>(pos.y) + m_SpriteXYOffset.second / 2 - m_CurrentFlameRect.h / 2,
			m_CurrentFlameRect.w, m_CurrentFlameRect.h);
		break;
	case RenderComponent::LEFT:
		Renderer::GetInstance().RenderRect(GetWorldFlameRectCollision(render));
		return SDL_Rect(static_cast<int>(pos.x),
			static_cast<int>(pos.y) + m_CurrentFlameRect.h + m_SpriteXYOffset.second / 2 - m_CurrentFlameRect.h / 2,
			m_CurrentFlameRect.w, m_CurrentFlameRect.h);
		break;
	case RenderComponent::UP:
		Renderer::GetInstance().RenderRect(GetWorldFlameRectCollision(render));
		return SDL_Rect(static_cast<int>(pos.x) + m_SpriteXYOffset.second / 2 - m_CurrentFlameRect.h / 2,
			static_cast<int>(pos.y),
			m_CurrentFlameRect.w, m_CurrentFlameRect.h);
		break;
	case RenderComponent::DOWN:
		Renderer::GetInstance().RenderRect(GetWorldFlameRectCollision(render));
		return SDL_Rect(static_cast<int>(pos.x) + m_SpriteXYOffset.first - m_SpriteXYOffset.second / 2 + m_CurrentFlameRect.h / 2,
			static_cast<int>(pos.y) + m_SpriteXYOffset.first,
			m_CurrentFlameRect.w, m_CurrentFlameRect.h);
		break;
	}
	return {};
}

SDL_Rect dae::FygarAttackComponent::GetWorldFlameRectCollision(RenderComponent& render) {
	const auto& pos = BaseComponent::GetOwner()->GetWorldTransform().GetPosition();
	switch (render.GetDirection()) {
	case RenderComponent::RIGHT:
		return SDL_Rect(static_cast<int>(pos.x) + m_SpriteXYOffset.first,
			static_cast<int>(pos.y) + m_SpriteXYOffset.second / 2 - m_CurrentFlameRect.h / 2,
			m_CurrentFlameRect.w, m_CurrentFlameRect.h);
		break;
	case RenderComponent::LEFT:
		return SDL_Rect(static_cast<int>(pos.x) - m_CurrentFlameRect.w,
			static_cast<int>(pos.y) + m_SpriteXYOffset.second / 2 - m_CurrentFlameRect.h / 2,
			m_CurrentFlameRect.w, m_CurrentFlameRect.h);
		break;
	case RenderComponent::UP:
		return SDL_Rect(static_cast<int>(pos.x),
			static_cast<int>(pos.y) - m_CurrentFlameRect.w,
			m_CurrentFlameRect.h, m_CurrentFlameRect.w);
		break;
	case RenderComponent::DOWN:
		return SDL_Rect(static_cast<int>(pos.x)  + m_SpriteXYOffset.second / 2 - m_CurrentFlameRect.h / 2,
			static_cast<int>(pos.y) + m_SpriteXYOffset.first,
			m_CurrentFlameRect.h, m_CurrentFlameRect.w);
		break;
	}
	return {};
}
