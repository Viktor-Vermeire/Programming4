#include "KeyboardComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Minigin.h"
#include "iostream"
#include "TextComponent.h"

dae::KeyboardComponent::KeyboardComponent(GameObject& gameObject, float inputCoolDown): BaseComponent(gameObject), m_InputCooldown{inputCoolDown}
{
	m_KeyLocations = {};
	m_CooldownTimer = 0.5f;
	m_HighlightedLetter = { 0.f,0.f };
}

void dae::KeyboardComponent::Update()
{
	if (m_CooldownTimer >= 0) {
		m_CooldownTimer -= Minigin::DELTATIME;
	}
}

void dae::KeyboardComponent::AddKeyLocation(KeyInfo location)
{
	std::vector< KeyInfo> test = { location };
	m_KeyLocations.emplace_back(test);
}

void dae::KeyboardComponent::AddKeyLocation(int y, KeyInfo location)
{
	m_KeyLocations[y].emplace_back(location);
}

void dae::KeyboardComponent::ChangeHighlightedLetter(std::pair<float, float> change)
{
	if (m_CooldownTimer > 0) return;
	m_CooldownTimer = m_InputCooldown;
	std::cout << "key: " << m_HighlightedLetter.first << "y: " << m_HighlightedLetter.second << "\n";
	m_HighlightedLetter.second += change.second;
	if (m_HighlightedLetter.second >= m_KeyLocations.size())
		m_HighlightedLetter.second = 0;
	if (m_HighlightedLetter.second < 0) 
		m_HighlightedLetter.second = static_cast<float>(m_KeyLocations.size()) - 1.f;
	m_HighlightedLetter.first += change.first;
	if (m_HighlightedLetter.first >= m_KeyLocations[static_cast<int>(m_HighlightedLetter.second)].size())
		m_HighlightedLetter.first = 0;
	if (m_HighlightedLetter.first < 0)
		m_HighlightedLetter.first = static_cast<float>(m_KeyLocations[static_cast<int>(m_HighlightedLetter.second)].size()) - 1.f;
}

void dae::KeyboardComponent::SetHighLightSize(std::pair<int, int> size)
{
	m_HighlightSize = size;
}

void dae::KeyboardComponent::ConfirmLetter()
{
	if (m_CooldownTimer > 0) return;
	m_CooldownTimer = m_InputCooldown;
	auto check = GetKeyLocation(static_cast<int>(std::round(m_HighlightedLetter.first)), static_cast<int>(std::round(m_HighlightedLetter.second))).value;
	m_SavedString.append(check);
	auto textRender = GetOwner()->GetComponent<dae::TextComponent>();
	if (textRender)
		textRender->SetText(m_SavedString);
}

void dae::KeyboardComponent::Render()
{
	Renderer::GetInstance().RenderRect(SDL_Rect{
		static_cast<int>(std::round(m_KeyLocations[static_cast<int>(m_HighlightedLetter.second)][static_cast<int>(m_HighlightedLetter.first)].x)),
		static_cast<int>(std::round(m_KeyLocations[static_cast<int>(m_HighlightedLetter.second)][static_cast<int>(m_HighlightedLetter.first)].y)),
		m_HighlightSize.first,m_HighlightSize.second });
}

dae::KeyInfo dae::KeyboardComponent::GetKeyLocation(int x, int y)
{
	return m_KeyLocations[y][x];
}
