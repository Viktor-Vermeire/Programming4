#pragma once
#include "SoundSystem.h"
#include "memory"
#include <thread>
#include <SDL_mixer.h> 
#include "vector"

namespace dae {
	class SDLSoundSystem : public SoundSystem
	{
	public:
		SDLSoundSystem() = default;
		
		void AddEffect(std::vector<const char*> effects) override;
		void play(const unsigned short id, const float volume) override;
		void CleanupSoundThreads() override;
		
	private:
		void playSound(const unsigned short id, const float volume);
		std::vector<std::unique_ptr<Mix_Chunk>> m_Effects;
		std::vector<std::jthread> threads;

	};
}

