#pragma once
#include <BaseComponent.h>
#include "vector"
namespace dae {
	struct KeyInfo {
		float x;
		float y;
		std::string value;
	};
	class GameObject;
	class KeyboardComponent : public BaseComponent
	{
	public:
		KeyboardComponent(GameObject& gameObject, float inputCoolDown);
		void Update() override;
		void AddKeyLocation(const KeyInfo& location);
		void AddKeyLocation(int y, const KeyInfo& location);
		void ChangeHighlightedLetter(const std::pair<float, float>& change);
		void SetHighLightSize(const std::pair<int, int>& size);
		void ConfirmLetter();
		void SaveScore();
		void SetFileName(const std::string& fileName);
		void Render() override;
		KeyInfo GetKeyLocation(int x, int y);
	private:
		bool m_HasRead{ false };
		void ReadHighScoreBinary();
		std::unique_ptr<const char[]> IntToBytes(int value);
		std::vector < std::pair<std::string, int>> m_SavedScores;
		std::string m_FileName;
		float m_InputCooldown;
		float m_CooldownTimer;
		std::string m_SavedString{""};
		std::pair<int, int> m_HighlightSize;
		std::pair<float, float> m_HighlightedLetter;
		std::vector < std::vector<KeyInfo>> m_KeyLocations;
	};
}

