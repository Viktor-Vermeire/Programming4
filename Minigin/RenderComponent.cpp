#include "RenderComponent.h"
#include "GameObject.h"
#include "BaseComponent.h"
#include "ResourceManager.h"
#include "Texture2D.h"

dae::RenderComponent::RenderComponent(GameObject& gameObject):
	BaseComponent(gameObject)
{
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
	m_Box = SDL_Rect{};
}

void dae::RenderComponent::SetTexture(const std::string& filename, SDL_Rect box)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
	m_Box = box;
}

void dae::RenderComponent::Render()
{
	const auto& pos = BaseComponent::GetOwner()->GetWorldTransform().GetPosition();
	if (m_Box.w == 0 || m_Box.h == 0)
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	else {
		Renderer::GetInstance().RenderTexture(*m_Texture, m_Box, SDL_Rect(static_cast<int>(pos.x), static_cast<int>(pos.y), m_Box.w, m_Box.h));
	}
}
