#pragma once
#include "Command.h"
#include <utility>
namespace dae {
    class GameObject;
    class Move : public Command
    {
    public:
        Move(int inputValue, bool usingGamepad, std::pair<float, float> direction);
        void execute(GameObject& gameObject);
    private:
        std::pair<float, float> m_Direction;
    };
}

