#pragma once
#include "State.h"
#include "memory"

namespace dae {
    class Gamepad;
    class GameObject;
    class HandleInput : public State
    {
    public:
        HandleInput(const Gamepad& gamePad);
        void Enter(GameObject* ) override;
        void Execute(GameObject* gameObject) override;
        void Exit(GameObject* ) override;
    private :
        std::unique_ptr<Gamepad> m_GamePad;
    };
}

