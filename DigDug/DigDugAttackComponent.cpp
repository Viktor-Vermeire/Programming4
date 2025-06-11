#include "DigDugAttackComponent.h"
#include "Minigin.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "iostream"

dae::DigDugAttackComponent::DigDugAttackComponent(GameObject& go, SDL_Rect maxAttackRect, std::string texturePath, float maxGrowTime)
	: BaseComponent(go), 
m_MaxAttackRect{maxAttackRect}, m_MaxGrowTime{maxGrowTime}
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

void dae::DigDugAttackComponent::Update()
{
	if (!m_Active) return;

	if (m_Growing) {
		m_TimeGrowing += Minigin::DELTATIME;
		std::cout << "time passed: " << Minigin::DELTATIME << "\n max Time: " << m_MaxGrowTime << "\n";
		if (m_TimeGrowing > m_MaxGrowTime) {
			m_Growing = false;
			m_TimeGrowing = m_MaxGrowTime;
		}
		m_CurrentAttackRect.w = static_cast<int>(std::roundf(m_MaxAttackRect.w * (m_TimeGrowing / m_MaxGrowTime)));
		m_CurrentAttackRect.x = (m_MaxAttackRect.x + m_MaxAttackRect.w - m_CurrentAttackRect.w);
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
			const auto& pos = BaseComponent::GetOwner()->GetWorldTransform().GetPosition();
			switch (spriteRenderer->GetDirection()) {
			case RenderComponent::RIGHT:
				Renderer::GetInstance().RenderTexture(*m_Texture, 
					SDL_Rect(m_CurrentAttackRect.x, m_CurrentAttackRect.y, m_CurrentAttackRect.w, m_CurrentAttackRect.h),
					SDL_Rect(static_cast<int>(pos.x) + m_SpriteXYOffset.first, 
						static_cast<int>(pos.y) + m_SpriteXYOffset.second / 2 - m_CurrentAttackRect.h / 2, 
						m_CurrentAttackRect.w, m_CurrentAttackRect.h));
				break;
			case RenderComponent::LEFT:
				Renderer::GetInstance().RenderTextureOriented(*m_Texture, 
					SDL_Rect(m_CurrentAttackRect.x, m_CurrentAttackRect.y, m_CurrentAttackRect.w, m_CurrentAttackRect.h),
					SDL_Rect(static_cast<int>(pos.x), 
						static_cast<int>(pos.y) + m_CurrentAttackRect.h + m_SpriteXYOffset.second / 2 - m_CurrentAttackRect.h / 2,
						m_CurrentAttackRect.w, m_CurrentAttackRect.h),
					180, SDL_FLIP_VERTICAL, false);
				break;
			case RenderComponent::UP:
				Renderer::GetInstance().RenderTextureOriented(*m_Texture, 
					SDL_Rect(m_CurrentAttackRect.x, m_CurrentAttackRect.y, m_CurrentAttackRect.w, m_CurrentAttackRect.h),
					SDL_Rect(static_cast<int>(pos.x)  + m_SpriteXYOffset.second / 2 - m_CurrentAttackRect.h / 2,
						static_cast<int>(pos.y),
						m_CurrentAttackRect.w, m_CurrentAttackRect.h), 
					-90, SDL_FLIP_NONE, false);
				break;
			case RenderComponent::DOWN:
				Renderer::GetInstance().RenderTextureOriented(*m_Texture, 
					SDL_Rect(m_CurrentAttackRect.x, m_CurrentAttackRect.y, m_CurrentAttackRect.w, m_CurrentAttackRect.h),
					SDL_Rect(static_cast<int>(pos.x) + m_SpriteXYOffset.first - m_SpriteXYOffset.second / 2 + m_CurrentAttackRect.h / 2,
						static_cast<int>(pos.y) + m_SpriteXYOffset.first , 
						m_CurrentAttackRect.w,m_CurrentAttackRect.h),
					90, SDL_FLIP_NONE, false);
				break;
			}
		}
	}
}
