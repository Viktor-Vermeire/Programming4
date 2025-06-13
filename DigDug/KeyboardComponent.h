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
		void AddKeyLocation(KeyInfo location);
		void AddKeyLocation(int y, KeyInfo location);
		void ChangeHighlightedLetter(std::pair<float, float> change);
		void SetHighLightSize(std::pair<int, int> size);
		void ConfirmLetter();
		void SaveScore();
		void SetFileName(std::string fileName);
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

