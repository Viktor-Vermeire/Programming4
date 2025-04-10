#include "TextComponent.h"
#include <stdexcept>
#include "Minigin.h"

#include "Font.h"
#include "Renderer.h"
#include "GameObject.h"
#include <SDL_ttf.h>
#include "Transform.h"

#include "Texture2D.h"
dae::TextComponent::TextComponent(GameObject& gameObject, const std::string& text, std::shared_ptr<Font> font):
	BaseComponent(gameObject),m_Text{ text }, m_pFont(std::move(font)), m_pTextTexture(nullptr)
{
	m_NeedsUpdate = true;
}

void dae::TextComponent::Update()
{
	if (m_NeedsUpdate) {

		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_pTextTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::TextComponent::Render()
{
	if (m_pTextTexture != nullptr)
	{
		const auto& pos = BaseComponent::GetOwner()->GetWorldTransform().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_pTextTexture, pos.x, pos.y);
	}
}
