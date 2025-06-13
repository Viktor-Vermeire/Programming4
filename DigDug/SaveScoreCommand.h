#pragma once
#include <Command.h>
namespace dae {
	class SaveScoreCommand : public Command
	{
    public:
        SaveScoreCommand(int inputValue, bool usingGamepad);
        void execute(GameObject&) override;
    private:
	};
}
