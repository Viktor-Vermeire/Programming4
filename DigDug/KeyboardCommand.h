#pragma once
#include <Command.h>
#include "vector"
namespace dae {
	class KeyboardMoveCommand : public Command
	{
    public:
        KeyboardMoveCommand(int inputValue, bool usingGamepad, std::pair<float, float> change);
        void execute(GameObject& gameObject);
    private:
        std::pair<float, float> m_Change;
    };
    class KeyboardConfirmCommand : public Command
    {
    public:
        KeyboardConfirmCommand(int inputValue, bool usingGamepad);
        void execute(GameObject& gameObject);
    private:
    };
}
