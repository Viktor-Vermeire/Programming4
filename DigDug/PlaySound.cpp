#include "PlaySound.h"
#include "ServiceLocator.h"
namespace dae {
	PlaySound::PlaySound(int inputValue, bool usingGamepad, int soundId): Command(inputValue, usingGamepad), 
		m_SoundId {soundId}
	{
	}
	void PlaySound::execute(GameObject&)
	{
		ServiceLocator::get_SoundSystem().PlayEffect(static_cast<const unsigned short>(m_SoundId), 100);
	}
}