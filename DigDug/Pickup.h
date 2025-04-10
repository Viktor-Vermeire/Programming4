#pragma once
#include "Command.h"
namespace dae {
    class Pickup :
        public Command
    {
    public:
        Pickup(int inputValue, bool usingGamepad, int scoreValue);
        void execute(GameObject& gameObject) override;
    private:
        int m_ScoreValue;
    };
}