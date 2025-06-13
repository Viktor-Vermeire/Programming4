#pragma once
#include <Command.h>
namespace dae {
    class FygarAttackCommand : public Command
    {
    public:
        FygarAttackCommand(int inputValue, bool usingGamepad);
        void execute(GameObject& gameObject);
    };
}
