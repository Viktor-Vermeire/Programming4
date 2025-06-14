#pragma once
#include "vector"
namespace dae {
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void PlayEffect(const unsigned short id, const float volume) = 0;
		virtual void PlayMusic(const unsigned short id, const float volume) = 0;
		virtual void AddEffects(std::vector<const char*> effects) = 0;
		virtual void AddSongs(std::vector<const char*> effects) = 0;
		virtual void ToggleMute() = 0;
		// ...and other relevant methods of course...
	};
}