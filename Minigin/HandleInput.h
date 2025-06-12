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
        HandleInput(Gamepad* gamePad);
        void Enter(GameObject* ) override;
        void Execute(GameObject* gameObject) override;
        void Exit(GameObject* ) override;
        void AddCommand(Command* command);
    private :
        Gamepad* m_GamePad;
        std::vector<Command*> m_Commands;
        
    };
}

