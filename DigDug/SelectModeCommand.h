#pragma once
#include <Command.h>
#include "string"
namespace dae {
    class SelectModeCommand :
        public Command
    {
    public:
        SelectModeCommand(unsigned int inputValue, bool usingGamepad, const std::string& sceneName, int songIndex);
        void execute(GameObject&) override;
    private:
        std::string m_SceneName;
        unsigned short m_SongIndex;
    };
}
