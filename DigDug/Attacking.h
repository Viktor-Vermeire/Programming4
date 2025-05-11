#pragma once
#include <State.h>
namespace dae {
    class Attacking :
        public State
    {
        ~Attacking() = default;
        void Enter(GameObject*)override;
        void Execute(GameObject* gameObject)override;
        void Exit(GameObject*) override;
    };
}

