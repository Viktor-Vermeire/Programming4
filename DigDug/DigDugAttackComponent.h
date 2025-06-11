#pragma once
#include <BaseComponent.h>
#include <SDL_ttf.h>
namespace dae {
    class Texture2D;
    class DigDugAttackComponent :
        public BaseComponent
    {
    public:
        DigDugAttackComponent(GameObject& go, SDL_Rect maxAttackRect, std::string texturePath, float maxGrowTime);
        bool IsActive();
        void SetActive(bool active);
        void SetGrowing(bool growing);
        void SetTimeGrowing(float time);
        void Update() override;
        void Render() override;
    private:
        bool m_Active;
        bool m_Growing;
        float m_TimeGrowing;
        float m_MaxGrowTime;
        bool m_Connected;
        std::pair<int, int> m_SpriteXYOffset;
        Texture2D* m_Texture;
        SDL_Rect m_MaxAttackRect;
        SDL_Rect m_CurrentAttackRect;


    };
}

