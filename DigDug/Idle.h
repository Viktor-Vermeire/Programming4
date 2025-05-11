#pragma once
#include <State.h>
namespace dae {
    class Idle :
        public State
    {
        ~Idle() = default;
        void Enter(GameObject* )override;
        void Execute(GameObject* gameObject)override;
        void Exit(GameObject* ) override;

    };
}

