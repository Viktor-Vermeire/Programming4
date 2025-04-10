#include "HallwaysComponent.h"
#include "BaseComponent.h"
#include "GameObject.h"
#include "Texture2D.h"
#include <SDL_ttf.h>
#include "vector"
#include "ResourceManager.h"
#include "Renderer.h"
#include "RenderComponent.h"
#include "windows.h"

dae::HallwaysComponent::HallwaysComponent(GameObject& gameObject, int width, int height) :
	BaseComponent(gameObject) {
	for (int outerLooper{ 0 }; outerLooper < height/8; ++outerLooper) {
		m_HasBeenDug.emplace_back();
		for (int innerLooper{ 0 }; innerLooper < width/8; ++innerLooper) {
			m_HasBeenDug.at(m_HasBeenDug.size() - 1).emplace_back(false);
		}
	}
}

void dae::HallwaysComponent::SetTexture(const std::string& filename, SDL_Rect box)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
	m_HasBeenDug.at(16).at(24) = true;
	m_HasBeenDug.at(16).at(25) = true;
	m_HasBeenDug.at(17).at(24) = true;
	m_HasBeenDug.at(17).at(25) = true;

	m_HasBeenDug.at(23).at(24) = true;
	m_HasBeenDug.at(23).at(25) = true;
	m_HasBeenDug.at(24).at(24) = true;
	m_HasBeenDug.at(24).at(25) = true;

	m_HasBeenDug.at(48).at(24) = true;
	m_HasBeenDug.at(48).at(25) = true;
	m_HasBeenDug.at(49).at(24) = true;
	m_HasBeenDug.at(49).at(25) = true;
	m_Box = box;
}

void dae::HallwaysComponent::AddPlayer(GameObject* player)
{
	m_Players.emplace_back(player);
}

void dae::HallwaysComponent::Update()
{
	for (auto player : m_Players) {
		if (player->GetComponent<dae::RenderComponent>()) {
			if (player->GetComponent<dae::RenderComponent>()->GetBox().w != 0 && player->GetComponent<dae::RenderComponent>()->GetBox().h != 0) {
				RECT intersectRect;
				//player->GetWorldTransform     WErk dit verder uit
			}
		}
	}
}


void dae::HallwaysComponent::Render()
{
	SDL_SetTextureBlendMode(m_Texture->GetSDLTexture(), SDL_BLENDMODE_BLEND);
	Uint8 opacity = 128; // 50% opacity
	SDL_SetTextureAlphaMod(m_Texture->GetSDLTexture(), opacity);
	for (int outerLooper{ 0 }; outerLooper < m_HasBeenDug.size(); ++outerLooper) {
		for (int innerLooper{ 0 }; innerLooper < m_HasBeenDug.at(outerLooper).size(); ++innerLooper) {
			if (m_HasBeenDug.at(outerLooper).at(innerLooper))
				Renderer::GetInstance().RenderTexture(*m_Texture, m_Box, SDL_Rect(innerLooper * 8, outerLooper * 8, m_Box.w, m_Box.h));
		}
	}
}
