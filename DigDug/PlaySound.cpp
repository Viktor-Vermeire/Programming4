#include "PlaySound.h"
#include "ServiceLocator.h"
namespace dae {
	PlaySound::PlaySound(int inputValue, bool usingGamepad, int soundId): Command(inputValue, usingGamepad), 
		m_SoundId {soundId}
	{
	}
	void PlaySound::execute(GameObject& gameObject)
	{
		ServiceLocator::get_SoundSystem().play(m_SoundId, 100);
	}
}