#pragma once

#pragma once
#include "BaseComponent.h"
#include "TextComponent.h"
#include "Renderer.h"
#include "RenderComponent.h"

#include "map"
#include "vector"
#include "Font.h"
#include <SDL_ttf.h>
namespace dae {
    class Texture2D;
    class AnimationComponent : public BaseComponent
    {
    public:
        enum AnimationState {
            IDLE,RUNNING, ATTACKING
        };
        AnimationComponent(GameObject& gameObject);
        AnimationState GetAnimationState();
        void SetCurrentAnimationState(AnimationState state);
        void SetDirection(RenderComponent::Direction direction);
        void Update() override;
        void SetTexture(const std::string& filename);
        void AddAnimationStateLocations(AnimationState state, std::vector<SDL_Rect> locations);
        void AddAnimationStateDuration(AnimationState state, float duration);
        //SDL_Rect GetBox();
        void Render() override;
    private:
        AnimationState m_CurrentState;
        RenderComponent::Direction m_Direction;
        std::shared_ptr<Texture2D> m_Texture{};
        std::map<AnimationState, std::vector<SDL_Rect>> m_AnimationLocations;
        std::map<AnimationState, float> m_AnimationFrameDuration;
        float m_TimeInAnimation;
        

    };
}

