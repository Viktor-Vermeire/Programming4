#pragma once
#include "memory"
namespace dae
{
    class GamePadImpl;
    class Gamepad
    {
    public:
        Gamepad();
        Gamepad(int a_iIndex);

        // Utility functions
        //XINPUT_STATE GetState(); // Return gamepad state
        int GetIndex();          // Return gamepad index
        bool Connected();        // Return true if gamepad is connected
        void Update();           // Update gamepad state
        bool IsUsed();
        void SetUsed(bool used);

        
        bool IsDownThisFrame(unsigned int button) const;
        bool IsUpThisFrame(unsigned int button) const;
        bool IsPressed(unsigned int button) const;
    private:
        std::unique_ptr<GamePadImpl> pimpl;
    };
}

