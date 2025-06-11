#pragma once
#include <Command.h>
namespace dae {
    class GameObject;
    class DigDugAttackCommand : public Command
    {
    public:
        DigDugAttackCommand(int inputValue, bool usingGamepad);
        void execute(GameObject& gameObject);
    };
};

