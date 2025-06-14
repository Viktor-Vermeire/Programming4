#pragma once
#include <BaseComponent.h>
#include <SDL_ttf.h>
#include "vector"
namespace dae {
    class Texture2D;
    class RenderComponent;
    class GameObject;
    class FygarAttackComponent : public BaseComponent{
    public:
        FygarAttackComponent(GameObject& gameObject, const std::vector<SDL_Rect>& flameRects, const std::string& texturePath, float maxFlameTime);
        bool IsActive();
        void SetActive(bool active);
        void Update() override;
        void Render() override;
    private:
        SDL_Rect GetWorldFlameRectCollision(RenderComponent& render);
        SDL_Rect GetWorldFlameRect(RenderComponent& render);
        bool m_Active;
        float m_TimeFlaming;
        float m_MaxFlameTime;
        std::pair<int, int> m_SpriteXYOffset;
        Texture2D* m_Texture;
        std::vector<SDL_Rect> m_FlameRects;
        SDL_Rect m_CurrentFlameRect;
    };
}

