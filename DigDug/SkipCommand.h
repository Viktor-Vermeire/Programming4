#pragma once
#include <Command.h>
namespace dae {
    class SkipCommand :
        public Command
    {
    public:
        SkipCommand(unsigned int inputValue, bool usingGamepad);
        void execute(GameObject& gameObject) override;
    };
}

