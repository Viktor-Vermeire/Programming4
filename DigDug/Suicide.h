#pragma once
#include "Command.h"
namespace dae {
    class Suicide :
        public Command
    {
    public:
        Suicide(int inputValue, bool usingGamepad);
        void execute(GameObject& gameObject) override;
    private:
    };
}

