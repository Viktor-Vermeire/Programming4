#include "ToggleSoundCommand.h"
#include "ServiceLocator.h"



dae::ToggleSoundCommand::ToggleSoundCommand(unsigned int inputValue, bool usingGamepad): Command(inputValue, usingGamepad)
{
}

void dae::ToggleSoundCommand::execute(GameObject&)
{
	ServiceLocator::get_SoundSystem().ToggleMute();
}
