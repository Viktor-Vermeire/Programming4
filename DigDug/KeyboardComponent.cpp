#include "KeyboardComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Minigin.h"
#include "iostream"
#include "TextComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "PlayerComponent.h"
#include "Scene.h"
#include <fstream>
#include "algorithm"
#include "filesystem"
#include <array>

dae::KeyboardComponent::KeyboardComponent(GameObject& gameObject, float inputCoolDown): BaseComponent(gameObject), m_InputCooldown{inputCoolDown}
{
	m_KeyLocations = {};
	m_CooldownTimer = 0.5f;
	m_HighlightedLetter = { 0.f,0.f };
}

void dae::KeyboardComponent::Update()
{
	if (!m_HasRead) {
		ReadHighScoreBinary();
		m_HasRead = true;
		auto results = SceneManager::GetInstance().GetActiveScene()->findGameObjectsWithComponent<PlayerComponent>();
		auto textResults = SceneManager::GetInstance().GetActiveScene()->findGameObjectsWithComponent<TextComponent>();
		if (!results[0] && textResults[0]) return;
		int looper{ 0 };
		for (auto player : results) {
			auto gameObject = std::make_unique<GameObject>();
			gameObject->SetPosition(300.f , 180.f + (looper * 36.f));
			gameObject->AddComponent<dae::TextComponent>(*gameObject.get(), "Player " + std::to_string(looper+1) + ": " +
				std::to_string(player->GetComponent<PlayerComponent>()->GetScore()), textResults[0]->GetComponent<TextComponent>()->GetFont());
			SceneManager::GetInstance().GetActiveScene()->Add(std::move(gameObject));
			++looper;
		}
	}
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
	if (textRender) {
		textRender->SetText(m_SavedString);
	}
}

void dae::KeyboardComponent::SetFileName(std::string fileName)
{
	m_FileName = fileName;
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

std::unique_ptr<const char[]> dae::KeyboardComponent::IntToBytes(int value)
{
	auto bytes = std::make_unique<char[]>(sizeof(int));
	for (size_t i = 0; i < sizeof(int); ++i) {
		bytes[i] = static_cast<char>((value >> (i * 8)) & 0xFF);
	}
	return bytes;
}

void dae::KeyboardComponent::SaveScore()
{
	if (m_CooldownTimer > 0) return;
	m_CooldownTimer = m_InputCooldown;
	auto results = SceneManager::GetInstance().GetActiveScene()->findGameObjectsWithComponent<PlayerComponent>();
	if (!results[0]) return;

	std::ifstream inFile(m_FileName, std::ios::binary);
	if (inFile) {
		inFile.close();
		std::filesystem::remove(m_FileName);
	}

	std::ofstream outFile(m_FileName, std::ios::binary | std::ios::app);
	//if (!outFile) ;
	m_SavedScores.emplace_back(m_SavedString, results[0]->GetComponent<PlayerComponent>()->GetScore());
	std::sort(m_SavedScores.begin(), m_SavedScores.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
		return a.second < b.second;
		});

	
	int looper{ 0 };
	while (looper < 10 && looper < m_SavedScores.size()) {
		char tag = 'P';
		outFile.write(&tag, sizeof(tag));

		outFile.write(m_SavedScores[looper].first.c_str(), m_SavedScores[looper].first.size());
		char terminator = '\0';
		outFile.write(&terminator, sizeof(char));
		auto test = IntToBytes(m_SavedScores[looper].second);
		outFile.write(test.get(), sizeof(int));
		++looper;
	}
	

	outFile.close();
}

void dae::KeyboardComponent::ReadHighScoreBinary()
{
	std::ifstream inFile(m_FileName, std::ios::binary);
	if (!inFile) return;

	std::cout << "\nSaved Players (Binary)\n";

	while (inFile)
	{
		char tag{};
		inFile.read(&tag, sizeof(tag));
		if (!inFile || tag != 'P') break;

		std::string name{};
		char c{};
		while (inFile.read(&c, sizeof(char)) && c != '\0') {
			name += c;
		}

		int score{};
		if (!inFile.read(reinterpret_cast<char*>(&score), sizeof(int))) break;

		m_SavedScores.emplace_back(std::pair{ name, score });

		std::cout << "Name: " << name << ", Score: " << score << '\n';
	}

	inFile.close();
	auto results = SceneManager::GetInstance().GetActiveScene()->findGameObjectsWithComponent<TextComponent>();
	if (!results[0]) return;
	auto font = results[0]->GetComponent<TextComponent>()->GetFont();
	std::vector<GameObject*> objects{};
	for (int looper{ 0 }; looper < m_SavedScores.size()  && looper < 7; ++looper) {
		auto gameObject = std::make_unique<GameObject>();
		gameObject->SetPosition(30.f, looper * 36.f);
		gameObject->AddComponent<dae::TextComponent>(*gameObject.get(), (looper + 1) + ") " + 
			m_SavedScores[looper].first + " : " + std::to_string(m_SavedScores[looper].second), font);
		objects.emplace_back( gameObject.get() );
		SceneManager::GetInstance().GetActiveScene()->Add(std::move(gameObject));
	}


}


