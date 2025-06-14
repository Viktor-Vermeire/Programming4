#pragma once
#include <Command.h>
#include "vector"
namespace dae {
	class KeyboardMoveCommand : public Command
	{
    public:
        KeyboardMoveCommand(unsigned int inputValue, bool usingGamepad,const std::pair<float, float>& change);
        void execute(GameObject& gameObject);
    private:
        std::pair<float, float> m_Change;
    };

    class KeyboardConfirmCommand : public Command
    {
    public:
        KeyboardConfirmCommand(unsigned int inputValue, bool usingGamepad);
        void execute(GameObject& gameObject);
    private:
    };

    class SaveScoreCommand : public Command
    {
    public:
        SaveScoreCommand(unsigned int inputValue, bool usingGamepad);
        void execute(GameObject&) override;
    private:
    };
}
