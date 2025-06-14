#pragma once
#include <Command.h>
namespace dae {
    class ToggleSoundCommand :
        public Command
    {
    public:
        ToggleSoundCommand(unsigned int inputValue, bool usingGamepad);
        void execute(GameObject& gameObject);
    };
}
