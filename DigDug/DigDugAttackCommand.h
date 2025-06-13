#pragma once
#include <Command.h>
namespace dae {
    class GameObject;
    class DigDugAttackCommand : public Command
    {
    public:
        DigDugAttackCommand(unsigned int inputValue, bool usingGamepad);
        void execute(GameObject& gameObject);
    };
};

