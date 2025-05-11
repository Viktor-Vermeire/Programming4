#pragma once
#include "Command.h"
#include <utility>
#include "RenderComponent.h"
#include "MovementComponent.h"
namespace dae {
    class GameObject;
    class Move : public Command
    {
    public:
        Move(int inputValue, bool usingGamepad, RenderComponent::Direction direction);
        void execute(GameObject& gameObject);
    private:
        RenderComponent::Direction m_Direction;
    };
}

