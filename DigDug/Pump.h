#pragma once
#include <Command.h>
namespace dae {
    class Pump :
        public Command
    {
    public:
        Pump(unsigned int inputValue, bool usingGamepad);
        void execute(GameObject& gameObject) override;
    private:
    };
}

