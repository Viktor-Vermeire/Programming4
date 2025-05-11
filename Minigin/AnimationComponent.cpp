#include "AnimationComponent.h"
#include "Minigin.h"
#include "GameObject.h"
#include <cmath>

#include "ResourceManager.h"
dae::AnimationComponent::AnimationComponent(GameObject& gameObject) : BaseComponent(gameObject)
{
	m_CurrentState = IDLE;
	m_Direction = RenderComponent::RIGHT;
	m_TimeInAnimation = 0;
}

dae::AnimationComponent::AnimationState dae::AnimationComponent::GetAnimationState()
{
	return m_CurrentState;
}

void dae::AnimationComponent::SetCurrentAnimationState(AnimationState state)
{
	m_CurrentState = state;
	m_TimeInAnimation = 0;
}

void dae::AnimationComponent::SetDirection(RenderComponent::Direction direction)
{
	m_Direction = direction;
}

void dae::AnimationComponent::Update()
{
	m_TimeInAnimation += Minigin::DELTATIME;
	if (m_AnimationFrameDuration.at(m_CurrentState) < m_TimeInAnimation)
		m_TimeInAnimation -= m_AnimationFrameDuration.at(m_CurrentState);
}

void dae::AnimationComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::AnimationComponent::AddAnimationStateLocations(AnimationState state, std::vector<SDL_Rect> locations)
{
	m_AnimationLocations.insert(std::pair(state, locations));
}

void dae::AnimationComponent::AddAnimationStateDuration(AnimationState state, float duration)
{
	m_AnimationFrameDuration.insert(std::pair(state, duration));
}

void dae::AnimationComponent::Render()
{
	float duration = m_AnimationFrameDuration.at(m_CurrentState);
	auto& frames = m_AnimationLocations.at(m_CurrentState);
	float timePerFrame = duration / frames.size();
	int frameNumber = static_cast<int>((m_TimeInAnimation - std::fmod(m_TimeInAnimation, timePerFrame)) / timePerFrame);
	const auto& pos = BaseComponent::GetOwner()->GetWorldTransform().GetPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, frames.at(frameNumber), SDL_Rect(static_cast<int>(pos.x), static_cast<int>(pos.y), frames.at(frameNumber).w, frames.at(frameNumber).h));
}
