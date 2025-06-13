#pragma once
#include <Command.h>
namespace dae {
    class PlaySound :
        public Command
    {
    public:
        PlaySound(unsigned int inputValue, bool usingGamepad, int soundId);
        void execute(GameObject& gameObject);
    private:
        int m_SoundId;
    };
}

