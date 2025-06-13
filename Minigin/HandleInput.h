#pragma once
#include "State.h"
#include "memory"
#include "vector"

namespace dae {
    class Gamepad;
    class GameObject;
    class Command;
    class HandleInput : public State
    {
    public:
        HandleInput();
        void Enter(GameObject* ) override;
        void Execute(GameObject* gameObject) override;
        void Exit(GameObject* ) override;
        void AddCommand(Command* command);
        void AddGamepad(Gamepad* gamePad);
    private :
        std::vector<Gamepad*> m_Gamepads;
        std::vector<Command*> m_Commands;
        
    };
}

