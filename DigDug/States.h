#pragma once
#include <State.h>
#include <SDL_ttf.h>
#include <glm.hpp>
#include "vector"

namespace dae{
	class GameObject;
    class Command;
    class Gamepad;
    class Attacking :
        public State
    {
        ~Attacking() = default;
        void Enter(GameObject*)override;
        void Execute(GameObject* gameObject)override;
        void Exit(GameObject*) override;
    };

	class Running : public State
	{
    public:
        Running(std::vector<SDL_Rect> animationLocations, float timePerFrame);
		void Enter(GameObject* gameObject) override;
		void Execute(GameObject* gameObject) override;
		void Exit(GameObject* gameObject) override;
    private:
        float m_TimePerFrame = 0;
        float m_TimeInState = 0;
        std::vector<SDL_Rect> m_AnimationLocations;
	};

    class FloatingToPlayer : public State
    {
    public:
        FloatingToPlayer(std::vector<SDL_Rect> animationLocations, float timePerFrame);
        void Enter(GameObject*) override;
        void Execute(GameObject* gameObject) override;
        void Exit(GameObject*) override;
        float GetTimeInState();
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
        glm::vec3 m_Target;
    };
    
    class DigDugAttack : public State {
    public:
        DigDugAttack(Gamepad* gamePad);
        void Enter(GameObject* GameObject) override;
        void Execute(GameObject*) override;
        void Exit(GameObject*) override;
        void AddCommand(Command* command);
    private:
        std::vector<Command*> m_Commands;
        Gamepad* m_Gamepad;
    };

    class FygarAttack : public State {
    public:
        void Enter(GameObject*) override;
        void Execute(GameObject*) override;
        void Exit(GameObject*) override;
    private:
    };

    class Idle :
        public State
    {
    public:
        ~Idle() = default;
        void Enter(GameObject*)override;
        void Execute(GameObject* gameObject)override;
        void Exit(GameObject*) override;

    };

    class Tethered :
        public State
    {
    public:
        ~Tethered() = default;
        void Enter(GameObject*)override;
        void Execute(GameObject* gameObject)override;
        void Exit(GameObject*) override;

    };
}
