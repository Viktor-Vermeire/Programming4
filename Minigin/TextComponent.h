#pragma once
#include "BaseComponent.h"
#include "memory"
#include "Texture2D.h"
namespace dae
{
	class GameObject;
	class Font;
	class TextComponent : public BaseComponent
	{
	public:
		TextComponent(GameObject& gameObject, const std::string& text, std::shared_ptr<Font> font);
		void Update() override;
		void SetText(const std::string& text);
		void Render() override;
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = default;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = default;
	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		std::shared_ptr<Font> m_pFont;
		std::shared_ptr<Texture2D> m_pTextTexture;
		
	};
}

