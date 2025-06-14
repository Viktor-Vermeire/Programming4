#include "HallwaysComponent.h"
#include "BaseComponent.h"
#include "GameObject.h"
#include "Texture2D.h"
#include <glm.hpp>
#include "vector"
#include "ResourceManager.h"
#include "Renderer.h"
#include "RenderComponent.h"
#include "windows.h"
#include "map"
#include "iostream"

dae::HallwaysComponent::HallwaysComponent(GameObject& gameObject, int width, int height) :
	BaseComponent(gameObject) {
	for (int outerLooper{ 0 }; outerLooper < height/16; ++outerLooper) {
		m_Hallways.emplace_back();
		for (int innerLooper{ 0 }; innerLooper < width/16; ++innerLooper) {
			m_Hallways.at(m_Hallways.size() - 1).emplace_back(FILLED);
		}
	}
}

void dae::HallwaysComponent::ClearSky()
{
	for (int looper{ 0 };looper < m_Hallways.size() / 5; ++looper) {
		for (int innerLooper{ 0 }; innerLooper < m_Hallways.at(looper).size(); ++innerLooper) {
			if (looper == (m_Hallways.size() / 5) - 1) {
				m_Hallways.at(looper).at(innerLooper) = CLEARED;
			}
			else {
				m_Hallways.at(looper).at(innerLooper) = SKY;
			}
		}
		
	}
}

void dae::HallwaysComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::HallwaysComponent::Dig(const glm::vec3& fromLocation,const glm::vec3& toLocation)
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

void dae::HallwaysComponent::AddSource(HallwayType type,const SDL_Rect& source)
{
	m_Sources.insert(std::pair<HallwayType, SDL_Rect>(type, source));
}

const dae::HallwaysComponent::HallwayType& dae::HallwaysComponent::GetHallwayType(const glm::vec3& location)
{
	return m_Hallways.at(static_cast<int>(std::round(location.y / 16))).at(static_cast<int>(std::round(location.x / 16)));
}

bool dae::HallwaysComponent::IsValidHallway(const glm::vec3& location)
{
	int x = static_cast<int>(std::round(location.y / 16));
	int y = static_cast<int>(std::round(location.x / 16));
	if (x < 0 || x >= m_Hallways.size()) {
		return false;
	}
	if (y < 0 || y >= m_Hallways[x].size())
		return false;
	return m_Hallways[x][y] != SKY;
}

glm::vec3 dae::HallwaysComponent::GetFreeHallwayLocation(const glm::vec3& location)
{
	_int64 yindex = static_cast<_int64>(std::round(location.y / 16));
	_int64 xindex = static_cast<_int64>(std::round(location.x / 16));
	if (m_Hallways[yindex][xindex] != FILLED) {
		return glm::vec3{ xindex * 16,yindex * 16, 0 };
	}
	yindex--;
	xindex++;
	
	for (int looper{ 0 }; looper < 3; ++looper) {
		xindex -= 2;
		for (int innerlooper{ 0 }; innerlooper < 3; ++innerlooper) {
			if (m_Hallways[yindex][++xindex] != FILLED) {
				return glm::vec3{ xindex * 16,yindex * 16, 0 };
			}
		}
		++yindex;
	}
	//Als Ik ooit hier kom, big problem
	return glm::vec3();
}

void dae::HallwaysComponent::SetHallwayType(const std::pair<int, int>& index, HallwayType desiredType)
{
	m_Hallways.at(index.first).at(index.second) = desiredType;
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
			if (m_Hallways.at(outerLooper).at(innerLooper) != SKY)
				Renderer::GetInstance().RenderTexture(*m_Texture, m_Sources.at(m_Hallways.at(outerLooper).at(innerLooper)), 
					SDL_Rect(innerLooper * 16, outerLooper * 16, m_Sources.at(m_Hallways.at(outerLooper).at(innerLooper)).w, 
						m_Sources.at(m_Hallways.at(outerLooper).at(innerLooper)).h));
		}
	}
	opacity = 255;
	SDL_SetTextureAlphaMod(m_Texture->GetSDLTexture(), opacity);
}

void dae::HallwaysComponent::DigRight(const glm::vec3& fromLocation, const glm::vec3& toLocation)
{
	auto toYIndex = static_cast<_int64>(std::round(toLocation.y / 16));
	auto toXIndex = static_cast<_int64>(std::round(toLocation.x / 16));
	auto fromYIndex = static_cast<_int64>(std::round(fromLocation.y / 16));
	auto fromXIndex = static_cast<_int64>(std::round(fromLocation.x / 16));
	switch (m_Hallways.at(toYIndex).at(toXIndex)) {
	case FILLED: m_Hallways.at(toYIndex).at(toXIndex) = RIGHTCLOSED;
		break;
	case TOPCLOSED: m_Hallways.at(toYIndex).at(toXIndex) = RIGHTTOPCORNER;
		break;
	case VERTICALTHROUGH: m_Hallways.at(toYIndex).at(toXIndex) = CLEARED;
		break;
	case LEFTCLOSED: m_Hallways.at(toYIndex).at(toXIndex) = HORIZONTALTHROUGH;
		break;
	case BOTTOMCLOSED: m_Hallways.at(toYIndex).at(toXIndex) = RIGHTBOTTOMCORNER;
		break;
	case LEFTTOPCORNER:m_Hallways.at(toYIndex).at(toXIndex) = CLEARED;
		break;
	case LEFTBOTTOMCORNER:m_Hallways.at(toYIndex).at(toXIndex) = CLEARED;
		break;
	}
	switch (m_Hallways.at(fromYIndex).at(fromXIndex)) {
	case TOPCLOSED: m_Hallways.at(fromYIndex).at(fromXIndex) = LEFTTOPCORNER;
		break;
	case VERTICALTHROUGH: m_Hallways.at(fromYIndex).at(fromXIndex) = CLEARED;
		break;
	case RIGHTCLOSED: m_Hallways.at(fromYIndex).at(fromXIndex) = HORIZONTALTHROUGH;
		break;
	case BOTTOMCLOSED: m_Hallways.at(fromYIndex).at(fromXIndex) = LEFTBOTTOMCORNER;
		break;
	case RIGHTTOPCORNER:m_Hallways.at(fromYIndex).at(fromXIndex) = CLEARED;
		break;
	case RIGHTBOTTOMCORNER:m_Hallways.at(fromYIndex).at(fromXIndex) = CLEARED;
		break;
	}
}

void dae::HallwaysComponent::DigLeft(const glm::vec3& fromLocation, const glm::vec3& toLocation)
{
	auto toYIndex = static_cast<_int64>(std::round(toLocation.y / 16));
	auto toXIndex = static_cast<_int64>(std::round(toLocation.x / 16));
	auto fromYIndex = static_cast<_int64>(std::round(fromLocation.y / 16));
	auto fromXIndex = static_cast<_int64>(std::round(fromLocation.x / 16));
	switch (m_Hallways.at(toYIndex).at(toXIndex)) {
	case FILLED: m_Hallways.at(toYIndex).at(toXIndex) = LEFTCLOSED;
		break;
	case TOPCLOSED: m_Hallways.at(toYIndex).at(toXIndex) = LEFTTOPCORNER;
		break;
	case VERTICALTHROUGH: m_Hallways.at(toYIndex).at(toXIndex) = CLEARED;
		break;
	case RIGHTCLOSED: m_Hallways.at(toYIndex).at(toXIndex) = HORIZONTALTHROUGH;
		break;
	case BOTTOMCLOSED: m_Hallways.at(toYIndex).at(toXIndex) = LEFTBOTTOMCORNER;
		break;
	case RIGHTTOPCORNER:m_Hallways.at(toYIndex).at(toXIndex) = CLEARED;
		break;
	}
	switch (m_Hallways.at(fromYIndex).at(fromXIndex)) {
	case TOPCLOSED: m_Hallways.at(fromYIndex).at(fromXIndex) = RIGHTTOPCORNER;
		break;
	case VERTICALTHROUGH: m_Hallways.at(fromYIndex).at(fromXIndex) = CLEARED;
		break; //Check if this works
	case LEFTCLOSED: m_Hallways.at(fromYIndex).at(fromXIndex) = HORIZONTALTHROUGH;
		break;
	case BOTTOMCLOSED: m_Hallways.at(fromYIndex).at(fromXIndex) = RIGHTBOTTOMCORNER;
		break;
	case LEFTTOPCORNER:m_Hallways.at(fromYIndex).at(fromXIndex) = CLEARED;
		break;
	case LEFTBOTTOMCORNER:m_Hallways.at(fromYIndex).at(fromXIndex) = CLEARED;
		break;
	}
}

void dae::HallwaysComponent::DigUp(const glm::vec3& fromLocation, const glm::vec3& toLocation)
{
	auto toYIndex = static_cast<_int64>(std::round(toLocation.y / 16));
	auto toXIndex = static_cast<_int64>(std::round(toLocation.x / 16));
	auto fromYIndex = static_cast<_int64>(std::round(fromLocation.y / 16));
	auto fromXIndex = static_cast<_int64>(std::round(fromLocation.x / 16));
	switch (m_Hallways.at(toYIndex).at(toXIndex)) {
	case FILLED: m_Hallways.at(toYIndex).at(toXIndex) = TOPCLOSED;
		break;
	case BOTTOMCLOSED: m_Hallways.at(toYIndex).at(toXIndex) = VERTICALTHROUGH;
		break;
	case HORIZONTALTHROUGH: m_Hallways.at(toYIndex).at(toXIndex) = CLEARED;
		break;
	case LEFTCLOSED: m_Hallways.at(toYIndex).at(toXIndex) = LEFTTOPCORNER;
		break;
	case RIGHTCLOSED: m_Hallways.at(toYIndex).at(toXIndex) = RIGHTTOPCORNER;
		break;
	case LEFTBOTTOMCORNER:m_Hallways.at(toYIndex).at(toXIndex) = CLEARED;
		break;
	case RIGHTBOTTOMCORNER:m_Hallways.at(toYIndex).at(toXIndex) = CLEARED;
		break;
	}
	switch (m_Hallways.at(fromYIndex).at(fromXIndex)) {
	case TOPCLOSED: m_Hallways.at(fromYIndex).at(fromXIndex) = VERTICALTHROUGH;
		break;
	case HORIZONTALTHROUGH: m_Hallways.at(fromYIndex).at(fromXIndex) = CLEARED;
		break;
	case LEFTCLOSED: m_Hallways.at(fromYIndex).at(fromXIndex) = LEFTBOTTOMCORNER;
		break;
	case RIGHTCLOSED: m_Hallways.at(fromYIndex).at(fromXIndex) = RIGHTBOTTOMCORNER;
		break;
	case LEFTTOPCORNER:m_Hallways.at(fromYIndex).at(fromXIndex) = CLEARED;
		break;
	case RIGHTTOPCORNER:m_Hallways.at(fromYIndex).at(fromXIndex) = CLEARED;
		break;
	}
}

void dae::HallwaysComponent::DigDown(const glm::vec3& fromLocation, const glm::vec3& toLocation)
{
	auto toYIndex = static_cast<_int64>(std::round(toLocation.y / 16));
	auto toXIndex = static_cast<_int64>(std::round(toLocation.x / 16));
	auto fromYIndex = static_cast<_int64>(std::round(fromLocation.y / 16));
	auto fromXIndex = static_cast<_int64>(std::round(fromLocation.x / 16));
	switch (m_Hallways.at(toYIndex).at(toXIndex)) {
	case FILLED: m_Hallways.at(toYIndex).at(toXIndex) = BOTTOMCLOSED;
		break;
	case TOPCLOSED: m_Hallways.at(toYIndex).at(toXIndex) = VERTICALTHROUGH;
		break;
	case HORIZONTALTHROUGH: m_Hallways.at(toYIndex).at(toXIndex) = CLEARED;
		break;
	case LEFTCLOSED: m_Hallways.at(toYIndex).at(toXIndex) = LEFTBOTTOMCORNER;
		break;
	case RIGHTCLOSED: m_Hallways.at(toYIndex).at(toXIndex) = RIGHTBOTTOMCORNER;
		break;
	case LEFTTOPCORNER:m_Hallways.at(toYIndex).at(toXIndex) = CLEARED;
		break;
	case RIGHTTOPCORNER:m_Hallways.at(toYIndex).at(toXIndex) = CLEARED;
		break;
	}
	switch (m_Hallways.at(fromYIndex).at(fromXIndex)) {
	case BOTTOMCLOSED: m_Hallways.at(fromYIndex).at(fromXIndex) = VERTICALTHROUGH;
		break;
	case HORIZONTALTHROUGH: m_Hallways.at(fromYIndex).at(fromXIndex) = CLEARED;
		break;
	case LEFTCLOSED: m_Hallways.at(fromYIndex).at(fromXIndex) = LEFTTOPCORNER;
		break;
	case RIGHTCLOSED: m_Hallways.at(fromYIndex).at(fromXIndex) = RIGHTTOPCORNER;
		break;
	case LEFTBOTTOMCORNER:m_Hallways.at(fromYIndex).at(fromXIndex) = CLEARED;
		break;
	case RIGHTBOTTOMCORNER:m_Hallways.at(fromYIndex).at(fromXIndex) = CLEARED;
		break;
	}
}
