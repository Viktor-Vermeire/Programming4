#pragma once
#include "BaseComponent.h"
namespace dae {
    class GameObject;
    class TextComponent;
    class FPSComponent : public BaseComponent
    {
    public:
        FPSComponent(GameObject& gameObject);
        void Update() override;
        float GetFPS();
        void SetToWriteComponent(TextComponent* textComponent);
    private:
        void CalculateFPS();
        float m_FPS;
        TextComponent* m_ToWriteComponent;

    };
}

