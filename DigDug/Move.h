#pragma once
#include "Command.h"
#include <utility>
#include "AnimationComponent.h"
#include "MovementComponent.h"
namespace dae {
    class GameObject;
    class Move : public Command
    {
    public:
        Move(int inputValue, bool usingGamepad, AnimationComponent::Direction direction);
        void execute(GameObject& gameObject);
    private:
        AnimationComponent::Direction m_Direction;
    };
}

