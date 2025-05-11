#pragma once
#include <State.h>
#include <SDL_ttf.h>
#include "vector"
namespace dae {
	class FloatingToPlayer : public State
	{
    public:
        FloatingToPlayer(std::vector<SDL_Rect> animationLocations, float timePerFrame);
        void Enter(GameObject*) override;
        void Execute(GameObject* gameObject) override;
        void Exit(GameObject*) override;
    private:
        float m_TimePerFrame = 0;
        float m_TimeInState = 0;
        std::vector<SDL_Rect> m_AnimationLocations;
	};

    class FloatingToGrid : public State {
    public:
        FloatingToGrid(std::vector<SDL_Rect> animationLocations, float timePerFrame);
        void Enter(GameObject*) override;
        void Execute(GameObject* gameObject) override;
        void Exit(GameObject*) override;
    private:
        float m_TimePerFrame = 0;
        float m_TimeInState = 0;
        std::vector<SDL_Rect> m_AnimationLocations;
    };
}

