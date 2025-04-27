#pragma once
#include "vector"
namespace dae {
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void play(const unsigned short id, const float volume) = 0;
		virtual void AddEffect(std::vector<const char*> effects) = 0;
		virtual void CleanupSoundThreads() = 0;
		// ...and other relevant methods of course...
	};
}