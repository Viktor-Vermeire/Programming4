#include "RenderComponent.h"
#include "GameObject.h"
#include "BaseComponent.h"
#include "ResourceManager.h"
#include "Texture2D.h"

dae::RenderComponent::RenderComponent(GameObject& gameObject):
	BaseComponent(gameObject), m_Direction{RIGHT}
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

SDL_Rect dae::RenderComponent::GetBox()
{
	return m_Box;
}

void dae::RenderComponent::SetBox(SDL_Rect box)
{
	m_Box = box;
}

void dae::RenderComponent::Render()
{
	const auto& pos = BaseComponent::GetOwner()->GetWorldTransform().GetPosition();
	if (m_Box.w == 0 || m_Box.h == 0)
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	else {
		switch (m_Direction) {
		case RIGHT:
			Renderer::GetInstance().RenderTexture(*m_Texture, m_Box, SDL_Rect(static_cast<int>(pos.x), static_cast<int>(pos.y), m_Box.w, m_Box.h));
			break;
		case LEFT:
			Renderer::GetInstance().RenderTextureOriented(*m_Texture, m_Box, SDL_Rect(static_cast<int>(pos.x), static_cast<int>(pos.y), m_Box.w, m_Box.h), 0, SDL_FLIP_HORIZONTAL, true);
			break;
		case UP:
			Renderer::GetInstance().RenderTextureOriented(*m_Texture, m_Box, SDL_Rect(static_cast<int>(pos.x), static_cast<int>(pos.y), m_Box.w, m_Box.h), -90, SDL_FLIP_NONE, true);
			break;
		case DOWN:
			Renderer::GetInstance().RenderTextureOriented(*m_Texture, m_Box, SDL_Rect(static_cast<int>(pos.x), static_cast<int>(pos.y), m_Box.w, m_Box.h), 90, SDL_FLIP_NONE, true);
			break;
		}
		
	}
}

void dae::RenderComponent::SetDirection(Direction direction)
{
	m_Direction = direction;
}

dae::RenderComponent::Direction dae::RenderComponent::GetDirection() {
	return m_Direction;
}
