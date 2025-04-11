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
#include "map"

dae::HallwaysComponent::HallwaysComponent(GameObject& gameObject, int width, int height) :
	BaseComponent(gameObject) {
	for (int outerLooper{ 0 }; outerLooper < height/16; ++outerLooper) {
		m_Hallways.emplace_back();
		for (int innerLooper{ 0 }; innerLooper < width/16; ++innerLooper) {
			m_Hallways.at(m_Hallways.size() - 1).emplace_back(FILLED);
		}
	}
}

void dae::HallwaysComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
	m_Hallways.at(8).at(12) = LEFTTOPCORNER;
	m_Hallways.at(8).at(13) = RIGHTTOPCORNER;
	m_Hallways.at(9).at(12) = LEFTBOTTOMCORNER;
	m_Hallways.at(9).at(13) = RIGHTBOTTOMCORNER;

	m_Hallways.at(11).at(12) = CLEARED;
	m_Hallways.at(11).at(13) = CLEARED;
	m_Hallways.at(12).at(12) = CLEARED;
	m_Hallways.at(12).at(13) = CLEARED;

	m_Hallways.at(24).at(12) = VERTICALTHROUGH;
	m_Hallways.at(24).at(13) = HORIZONTALTHROUGH;
	m_Hallways.at(25).at(12) = LEFTCLOSED;
	m_Hallways.at(25).at(13) = RIGHTCLOSED;
}

void dae::HallwaysComponent::Dig(SDL_Rect fromLocation, SDL_Rect toLocation)
{
	if (fromLocation.x == toLocation.x) {
		if (fromLocation.y < toLocation.y) {
			DigDown(fromLocation, toLocation);
		}
		else {
			DigUp(fromLocation, toLocation);
		}
	}
	else{
		if (fromLocation.x < toLocation.x) {
			DigRight(fromLocation, toLocation);
		} 
		else {
			DigLeft(fromLocation, toLocation);
		}
	}
	
}

void dae::HallwaysComponent::AddSource(HallwayType type, SDL_Rect source)
{
	m_Sources.insert(std::pair<HallwayType, SDL_Rect>(type, source));
}

void dae::HallwaysComponent::Update()
{
	
}


void dae::HallwaysComponent::Render()
{
	SDL_SetTextureBlendMode(m_Texture->GetSDLTexture(), SDL_BLENDMODE_BLEND);
	Uint8 opacity = 128; // 50% opacity
	SDL_SetTextureAlphaMod(m_Texture->GetSDLTexture(), opacity);
	for (int outerLooper{ 0 }; outerLooper < m_Hallways.size(); ++outerLooper) {
		for (int innerLooper{ 0 }; innerLooper < m_Hallways.at(outerLooper).size(); ++innerLooper) {
			Renderer::GetInstance().RenderTexture(*m_Texture, m_Sources.at(m_Hallways.at(outerLooper).at(innerLooper)), SDL_Rect(innerLooper * 16, outerLooper * 16, m_Sources.at(m_Hallways.at(outerLooper).at(innerLooper)).w, m_Sources.at(m_Hallways.at(outerLooper).at(innerLooper)).h));
		}
	}
}

void dae::HallwaysComponent::DigRight(const SDL_Rect fromLocation, const SDL_Rect toLocation)
{
	switch (m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16)) {
	case FILLED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = RIGHTCLOSED;
		break;
	case TOPCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = RIGHTTOPCORNER;
		break;
	case VERTICALTHROUGH: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	case LEFTCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = HORIZONTALTHROUGH;
		break;
	case BOTTOMCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	case LEFTTOPCORNER:m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	case LEFTBOTTOMCORNER:m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	}
	switch (m_Hallways.at(fromLocation.y / 16).at(fromLocation.x / 16)) {
	case TOPCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = LEFTTOPCORNER;
		break;
	case VERTICALTHROUGH: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	case RIGHTCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = HORIZONTALTHROUGH;
		break;
	case BOTTOMCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = LEFTBOTTOMCORNER;
		break;
	case RIGHTTOPCORNER:m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	case RIGHTBOTTOMCORNER:m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	}
}

void dae::HallwaysComponent::DigLeft(const SDL_Rect fromLocation, const SDL_Rect toLocation)
{
	switch (m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16)) {
	case FILLED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = LEFTCLOSED;
		break;
	case TOPCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = LEFTTOPCORNER;
		break;
	case VERTICALTHROUGH: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	case RIGHTCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = HORIZONTALTHROUGH;
		break;
	case BOTTOMCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	case RIGHTTOPCORNER:m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	case RIGHTBOTTOMCORNER:m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	}
	switch (m_Hallways.at(fromLocation.y / 16).at(fromLocation.x / 16)) {
	case TOPCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = RIGHTTOPCORNER;
		break;
	case VERTICALTHROUGH: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	case LEFTCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = HORIZONTALTHROUGH;
		break;
	case BOTTOMCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = RIGHTBOTTOMCORNER;
		break;
	case LEFTTOPCORNER:m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	case LEFTBOTTOMCORNER:m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	}
}

void dae::HallwaysComponent::DigUp(const SDL_Rect fromLocation, const SDL_Rect toLocation)
{
	switch (m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16)) {
	case FILLED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = TOPCLOSED;
		break;
	case BOTTOMCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = VERTICALTHROUGH;
		break;
	case HORIZONTALTHROUGH: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	case LEFTCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = LEFTTOPCORNER;
		break;
	case RIGHTCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = RIGHTTOPCORNER;
		break;
	case LEFTBOTTOMCORNER:m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	case RIGHTBOTTOMCORNER:m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	}
	switch (m_Hallways.at(fromLocation.y / 16).at(fromLocation.x / 16)) {
	case TOPCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = VERTICALTHROUGH;
		break;
	case HORIZONTALTHROUGH: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	case LEFTCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = LEFTBOTTOMCORNER;
		break;
	case RIGHTCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = RIGHTBOTTOMCORNER;
		break;
	case LEFTTOPCORNER:m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	case RIGHTTOPCORNER:m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	}
}

void dae::HallwaysComponent::DigDown(const SDL_Rect fromLocation, const SDL_Rect toLocation)
{
	switch (m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16)) {
	case FILLED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = BOTTOMCLOSED;
		break;
	case TOPCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = VERTICALTHROUGH;
		break;
	case HORIZONTALTHROUGH: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	case LEFTCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = LEFTBOTTOMCORNER;
		break;
	case RIGHTCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = RIGHTBOTTOMCORNER;
		break;
	case LEFTTOPCORNER:m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	case RIGHTTOPCORNER:m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	}
	switch (m_Hallways.at(fromLocation.y / 16).at(fromLocation.x / 16)) {
	case BOTTOMCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = VERTICALTHROUGH;
		break;
	case HORIZONTALTHROUGH: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	case LEFTCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = LEFTTOPCORNER;
		break;
	case RIGHTCLOSED: m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = RIGHTTOPCORNER;
		break;
	case LEFTBOTTOMCORNER:m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	case RIGHTBOTTOMCORNER:m_Hallways.at(toLocation.y / 16).at(toLocation.x / 16) = CLEARED;
		break;
	}
}
